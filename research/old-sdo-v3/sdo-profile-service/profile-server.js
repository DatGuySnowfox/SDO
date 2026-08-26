'use strict';

const http = require('http');
const crypto = require('crypto');
const fs = require('fs');
const path = require('path');

const HOST = process.env.SDO_PROFILE_HOST || '0.0.0.0';
const PORT = Number(process.env.SDO_PROFILE_PORT || 27881);
const DATA_DIR = process.env.SDO_PROFILE_DATA_DIR || '/var/lib/surroundead-online-profiles';
const MAX_BYTES = Number(process.env.SDO_PROFILE_MAX_BYTES || 256 * 1024 * 1024);
const KEEP_REVISIONS = Number(process.env.SDO_PROFILE_KEEP_REVISIONS || 5);
const activeUploads = new Set();
const rateLimits = new Map();

fs.mkdirSync(DATA_DIR, { recursive: true, mode: 0o700 });

function nowIso() {
  return new Date().toISOString();
}

function sendJson(res, statusCode, payload, extraHeaders = {}) {
  const body = JSON.stringify(payload);
  res.writeHead(statusCode, {
    'Content-Type': 'application/json; charset=utf-8',
    'Content-Length': Buffer.byteLength(body),
    'Cache-Control': 'no-store',
    'X-Content-Type-Options': 'nosniff',
    ...extraHeaders,
  });
  res.end(body);
}

function cleanPlayerId(value) {
  const id = String(value || '');
  return /^[A-Za-z0-9_-]{8,64}$/.test(id) ? id : null;
}

function cleanDisplayName(value) {
  const text = String(value || 'Survivor')
    .replace(/[\r\n\t<>]/g, ' ')
    .replace(/\s+/g, ' ')
    .trim()
    .slice(0, 40);
  return text || 'Survivor';
}

function keyHash(key) {
  return crypto.createHash('sha256').update(String(key), 'utf8').digest('hex');
}

function safeEqualHex(a, b) {
  if (typeof a !== 'string' || typeof b !== 'string' || a.length !== b.length) return false;
  try {
    return crypto.timingSafeEqual(Buffer.from(a, 'hex'), Buffer.from(b, 'hex'));
  } catch {
    return false;
  }
}

function profilePaths(playerId) {
  const dir = path.join(DATA_DIR, playerId);
  return {
    dir,
    current: path.join(dir, 'current.zip'),
    meta: path.join(dir, 'meta.json'),
    revisions: path.join(dir, 'revisions'),
  };
}

function readMeta(playerId) {
  const p = profilePaths(playerId);
  if (!fs.existsSync(p.meta)) return null;
  try {
    const parsed = JSON.parse(fs.readFileSync(p.meta, 'utf8'));
    return parsed && typeof parsed === 'object' ? parsed : null;
  } catch {
    return null;
  }
}

function writeJsonAtomic(file, value) {
  const temp = `${file}.tmp-${process.pid}-${Date.now()}`;
  fs.writeFileSync(temp, JSON.stringify(value, null, 2), { mode: 0o600 });
  fs.renameSync(temp, file);
}

function authenticate(req, playerId, allowCreate) {
  const supplied = String(req.headers['x-profile-key'] || '');
  if (!/^[A-Fa-f0-9]{64,128}$/.test(supplied)) {
    return { ok: false, status: 401, error: 'missing_or_invalid_profile_key' };
  }

  const meta = readMeta(playerId);
  const suppliedHash = keyHash(supplied);

  if (!meta) {
    if (!allowCreate) return { ok: false, status: 404, error: 'profile_not_found' };
    return { ok: true, create: true, suppliedHash, meta: null };
  }

  if (!safeEqualHex(String(meta.keyHash || ''), suppliedHash)) {
    return { ok: false, status: 403, error: 'profile_key_rejected' };
  }

  return { ok: true, create: false, suppliedHash, meta };
}

function clientIp(req) {
  const forwarded = String(req.headers['x-forwarded-for'] || '').split(',')[0].trim();
  return forwarded || req.socket.remoteAddress || 'unknown';
}

function allowRequest(req) {
  const key = clientIp(req);
  const now = Date.now();
  let entry = rateLimits.get(key);
  if (!entry || now - entry.startedAt > 60_000) {
    entry = { startedAt: now, count: 0 };
    rateLimits.set(key, entry);
  }
  entry.count += 1;
  return entry.count <= 180;
}

function pruneRevisions(revisionsDir) {
  if (!fs.existsSync(revisionsDir)) return;
  const files = fs.readdirSync(revisionsDir)
    .filter((name) => name.endsWith('.zip'))
    .map((name) => {
      const full = path.join(revisionsDir, name);
      const stat = fs.statSync(full);
      return { full, mtimeMs: stat.mtimeMs };
    })
    .sort((a, b) => b.mtimeMs - a.mtimeMs);

  for (const item of files.slice(Math.max(0, KEEP_REVISIONS))) {
    try { fs.unlinkSync(item.full); } catch {}
  }
}

function streamUpload(req, tempFile) {
  return new Promise((resolve, reject) => {
    let bytes = 0;
    const hash = crypto.createHash('sha256');
    const out = fs.createWriteStream(tempFile, { flags: 'wx', mode: 0o600 });
    let completed = false;

    function fail(error) {
      if (completed) return;
      completed = true;
      try { out.destroy(); } catch {}
      try { fs.unlinkSync(tempFile); } catch {}
      reject(error);
    }

    out.on('error', fail);
    req.on('error', fail);

    req.on('data', (chunk) => {
      if (completed) return;
      bytes += chunk.length;
      if (bytes > MAX_BYTES) {
        fail(Object.assign(new Error('profile_too_large'), { statusCode: 413 }));
        // Keep consuming/discarding the request instead of resetting the upstream
        // socket. Resetting it made Nginx report a misleading 502 Bad Gateway.
        try { req.resume(); } catch {}
        return;
      }
      hash.update(chunk);
      if (!out.write(chunk)) req.pause();
    });

    out.on('drain', () => { if (!completed) req.resume(); });

    req.on('end', () => {
      if (completed) return;
      out.end(() => {
        if (completed) return;
        completed = true;
        resolve({ bytes, sha256: hash.digest('hex') });
      });
    });
  });
}

const server = http.createServer(async (req, res) => {
  if (!allowRequest(req)) {
    return sendJson(res, 429, { ok: false, error: 'rate_limited' });
  }

  const url = new URL(req.url || '/', `http://${req.headers.host || 'localhost'}`);

  if (req.method === 'GET' && url.pathname === '/health') {
    return sendJson(res, 200, {
      ok: true,
      service: 'SD-Online Server Profiles',
      version: '1.4.1',
      autosaveSeconds: 60,
      keepRevisions: KEEP_REVISIONS,
      maxProfileBytes: MAX_BYTES,
      serverTime: nowIso(),
    });
  }

  const match = url.pathname.match(/^\/v1\/profile\/([A-Za-z0-9_-]{8,64})(?:\/meta)?$/);
  if (!match) {
    return sendJson(res, 404, { ok: false, error: 'not_found' });
  }

  const playerId = cleanPlayerId(match[1]);
  const wantsMeta = url.pathname.endsWith('/meta');
  if (!playerId) return sendJson(res, 400, { ok: false, error: 'invalid_player_id' });

  if (req.method === 'GET' && wantsMeta) {
    const auth = authenticate(req, playerId, false);
    if (!auth.ok) return sendJson(res, auth.status, { ok: false, error: auth.error });
    const p = profilePaths(playerId);
    if (!fs.existsSync(p.current)) return sendJson(res, 404, { ok: false, error: 'profile_not_found' });
    const meta = auth.meta || readMeta(playerId);
    return sendJson(res, 200, {
      ok: true,
      playerId,
      displayName: meta.displayName,
      revision: meta.revision,
      sha256: meta.sha256,
      bytes: meta.bytes,
      updatedAt: meta.updatedAt,
    });
  }

  if (req.method === 'GET' && !wantsMeta) {
    const auth = authenticate(req, playerId, false);
    if (!auth.ok) return sendJson(res, auth.status, { ok: false, error: auth.error });
    const p = profilePaths(playerId);
    if (!fs.existsSync(p.current)) return sendJson(res, 404, { ok: false, error: 'profile_not_found' });
    const meta = auth.meta || readMeta(playerId) || {};
    const stat = fs.statSync(p.current);
    res.writeHead(200, {
      'Content-Type': 'application/zip',
      'Content-Length': stat.size,
      'Content-Disposition': `attachment; filename="${playerId}.zip"`,
      'Cache-Control': 'no-store',
      'X-Profile-Revision': String(meta.revision || 0),
      'X-Profile-Sha256': String(meta.sha256 || ''),
      'X-Profile-Updated-At': String(meta.updatedAt || ''),
      'X-Content-Type-Options': 'nosniff',
    });
    fs.createReadStream(p.current).pipe(res);
    return;
  }

  if (req.method === 'PUT' && !wantsMeta) {
    const auth = authenticate(req, playerId, true);
    if (!auth.ok) return sendJson(res, auth.status, { ok: false, error: auth.error });
    if (activeUploads.has(playerId)) return sendJson(res, 409, { ok: false, error: 'profile_upload_in_progress' });

    activeUploads.add(playerId);
    const p = profilePaths(playerId);
    fs.mkdirSync(p.dir, { recursive: true, mode: 0o700 });
    fs.mkdirSync(p.revisions, { recursive: true, mode: 0o700 });
    const temp = path.join(p.dir, `.upload-${process.pid}-${Date.now()}.zip`);

    try {
      const uploaded = await streamUpload(req, temp);
      if (uploaded.bytes < 22) {
        try { fs.unlinkSync(temp); } catch {}
        return sendJson(res, 400, { ok: false, error: 'empty_or_invalid_profile_archive' });
      }

      const claimedSha = String(req.headers['x-profile-sha256'] || '').toLowerCase();
      if (claimedSha && claimedSha !== uploaded.sha256) {
        try { fs.unlinkSync(temp); } catch {}
        return sendJson(res, 400, { ok: false, error: 'profile_checksum_mismatch' });
      }

      const oldMeta = auth.meta || readMeta(playerId);
      const nextRevision = Number(oldMeta && oldMeta.revision ? oldMeta.revision : 0) + 1;

      if (fs.existsSync(p.current)) {
        const stamp = new Date().toISOString().replace(/[:.]/g, '-');
        fs.renameSync(p.current, path.join(p.revisions, `revision-${String(nextRevision - 1).padStart(8, '0')}-${stamp}.zip`));
      }

      fs.renameSync(temp, p.current);
      const meta = {
        version: '1.4.1',
        playerId,
        displayName: cleanDisplayName(req.headers['x-display-name']),
        keyHash: auth.suppliedHash,
        revision: nextRevision,
        sha256: uploaded.sha256,
        bytes: uploaded.bytes,
        createdAt: oldMeta && oldMeta.createdAt ? oldMeta.createdAt : nowIso(),
        updatedAt: nowIso(),
      };
      writeJsonAtomic(p.meta, meta);
      pruneRevisions(p.revisions);

      return sendJson(res, 200, {
        ok: true,
        playerId,
        revision: nextRevision,
        sha256: uploaded.sha256,
        bytes: uploaded.bytes,
        updatedAt: meta.updatedAt,
      });
    } catch (error) {
      try { fs.unlinkSync(temp); } catch {}
      const status = Number(error && error.statusCode) || 500;
      console.error('[profile-upload]', playerId, error);
      return sendJson(res, status, { ok: false, error: error.message || 'upload_failed' });
    } finally {
      activeUploads.delete(playerId);
    }
  }

  return sendJson(res, 405, { ok: false, error: 'method_not_allowed' }, { Allow: 'GET, PUT' });
});

server.listen(PORT, HOST, () => {
  console.log(`SD-Online profile service 1.4.1 listening on http://${HOST}:${PORT}`);
  console.log(`Data directory: ${DATA_DIR}`);
});

function shutdown() {
  server.close(() => process.exit(0));
  setTimeout(() => process.exit(1), 5000).unref();
}
process.on('SIGTERM', shutdown);
process.on('SIGINT', shutdown);
