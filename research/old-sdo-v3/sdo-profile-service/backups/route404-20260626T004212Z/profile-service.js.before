'use strict';

const http = require('http');
const fs = require('fs');
const fsp = fs.promises;
const path = require('path');
const crypto = require('crypto');

const SERVICE = 'sdo-profile-service';
const VERSION = '1.0.0';
const HOST = process.env.HOST || '127.0.0.1';
const PORT = Number.parseInt(process.env.PORT || '27881', 10);
const STATE_DIR = process.env.STATE_DIR || '/var/lib/sdo-profile-service';
const PROFILES_DIR = path.join(STATE_DIR, 'profiles');
const MAX_UPLOAD_BYTES = Number.parseInt(process.env.MAX_UPLOAD_BYTES || String(256 * 1024 * 1024), 10);
const MAX_BACKUPS = Number.parseInt(process.env.MAX_BACKUPS || '10', 10);
const REGISTER_LIMIT_PER_HOUR = Number.parseInt(process.env.REGISTER_LIMIT_PER_HOUR || '12', 10);

const registrationWindows = new Map();

function nowIso() {
  return new Date().toISOString();
}

function sendJson(res, statusCode, body, extraHeaders = {}) {
  const data = Buffer.from(JSON.stringify(body));
  res.writeHead(statusCode, {
    'Content-Type': 'application/json; charset=utf-8',
    'Content-Length': String(data.length),
    'Cache-Control': 'no-store',
    ...extraHeaders
  });
  res.end(data);
}

function safeId(value) {
  return typeof value === 'string' && /^[A-Za-z0-9_-]{18,128}$/.test(value);
}

function safeToken(value) {
  return typeof value === 'string' && /^[a-f0-9]{64,128}$/i.test(value);
}

function cleanText(value, maxLength) {
  if (typeof value !== 'string') return '';
  return value.replace(/[\r\n\t\0]/g, ' ').trim().slice(0, maxLength);
}

function profileDir(playerId) {
  return path.join(PROFILES_DIR, playerId);
}

function metaPath(playerId) {
  return path.join(profileDir(playerId), 'metadata.json');
}

function currentPath(playerId) {
  return path.join(profileDir(playerId), 'current.zip');
}

function backupsDir(playerId) {
  return path.join(profileDir(playerId), 'backups');
}

function sha256Text(value) {
  return crypto.createHash('sha256').update(value, 'utf8').digest('hex');
}

function timingSafeHexEqual(left, right) {
  if (typeof left !== 'string' || typeof right !== 'string') return false;
  if (left.length !== right.length) return false;
  try {
    return crypto.timingSafeEqual(Buffer.from(left, 'hex'), Buffer.from(right, 'hex'));
  } catch {
    return false;
  }
}

async function readJson(filePath) {
  try {
    return JSON.parse(await fsp.readFile(filePath, 'utf8'));
  } catch (error) {
    if (error && error.code === 'ENOENT') return null;
    throw error;
  }
}

async function atomicWriteJson(filePath, value) {
  const temp = `${filePath}.${process.pid}.${Date.now()}.tmp`;
  await fsp.mkdir(path.dirname(filePath), { recursive: true, mode: 0o700 });
  await fsp.writeFile(temp, JSON.stringify(value, null, 2), { mode: 0o600 });
  await fsp.rename(temp, filePath);
}

function requestIp(req) {
  const forwarded = req.headers['x-forwarded-for'];
  if (typeof forwarded === 'string' && forwarded.trim()) {
    return forwarded.split(',')[0].trim();
  }
  return req.socket.remoteAddress || 'unknown';
}

function allowRegistration(req) {
  const ip = requestIp(req);
  const now = Date.now();
  const hourAgo = now - 60 * 60 * 1000;
  const existing = (registrationWindows.get(ip) || []).filter(value => value >= hourAgo);
  if (existing.length >= REGISTER_LIMIT_PER_HOUR) {
    registrationWindows.set(ip, existing);
    return false;
  }
  existing.push(now);
  registrationWindows.set(ip, existing);
  return true;
}

async function readJsonBody(req, maxBytes = 64 * 1024) {
  const chunks = [];
  let total = 0;
  for await (const chunk of req) {
    total += chunk.length;
    if (total > maxBytes) {
      const error = new Error('request-too-large');
      error.statusCode = 413;
      throw error;
    }
    chunks.push(chunk);
  }
  const text = Buffer.concat(chunks).toString('utf8');
  return JSON.parse(text || '{}');
}

function bearerToken(req) {
  const auth = req.headers.authorization;
  if (typeof auth !== 'string') return '';
  const match = /^Bearer\s+(.+)$/i.exec(auth.trim());
  return match ? match[1].trim() : '';
}

async function authenticate(req, playerId) {
  const meta = await readJson(metaPath(playerId));
  if (!meta) return { ok: false, statusCode: 404, error: 'profile-not-found' };
  const token = bearerToken(req);
  if (!safeToken(token)) return { ok: false, statusCode: 401, error: 'profile-token-required' };
  const candidate = sha256Text(token);
  if (!timingSafeHexEqual(candidate, meta.tokenHash)) {
    return { ok: false, statusCode: 403, error: 'profile-token-invalid' };
  }
  return { ok: true, meta };
}

async function registerProfile(req, res) {
  const body = await readJsonBody(req);
  const playerId = cleanText(body.playerId, 128);
  const profileToken = cleanText(body.profileToken, 128);
  const displayName = cleanText(body.displayName, 64) || 'Survivor';
  const launcherVersion = cleanText(body.launcherVersion, 32);

  if (!safeId(playerId) || !safeToken(profileToken)) {
    return sendJson(res, 400, { ok: false, error: 'invalid-player-id-or-token' });
  }

  const dir = profileDir(playerId);
  const metadataFile = metaPath(playerId);
  await fsp.mkdir(dir, { recursive: true, mode: 0o700 });
  await fsp.mkdir(backupsDir(playerId), { recursive: true, mode: 0o700 });

  let meta = await readJson(metadataFile);
  const tokenHash = sha256Text(profileToken);

  if (meta) {
    if (!timingSafeHexEqual(tokenHash, meta.tokenHash)) {
      return sendJson(res, 403, { ok: false, error: 'profile-already-registered' });
    }
    meta.displayName = displayName;
    meta.launcherVersion = launcherVersion;
    meta.lastRegisteredAt = nowIso();
    await atomicWriteJson(metadataFile, meta);
  } else {
    if (!allowRegistration(req)) {
      return sendJson(res, 429, { ok: false, error: 'registration-rate-limited' });
    }
    meta = {
      serviceVersion: VERSION,
      playerId,
      displayName,
      tokenHash,
      revision: 0,
      hasProfile: false,
      createdAt: nowIso(),
      updatedAt: null,
      lastRegisteredAt: nowIso(),
      launcherVersion,
      bytes: 0,
      sha256: null
    };
    await atomicWriteJson(metadataFile, meta);
  }

  return sendJson(res, 200, {
    ok: true,
    playerId,
    revision: meta.revision,
    hasProfile: meta.hasProfile,
    updatedAt: meta.updatedAt
  });
}

async function sendMeta(req, res, playerId) {
  const auth = await authenticate(req, playerId);
  if (!auth.ok) return sendJson(res, auth.statusCode, { ok: false, error: auth.error });
  const meta = auth.meta;
  return sendJson(res, 200, {
    ok: true,
    playerId,
    displayName: meta.displayName,
    revision: meta.revision,
    hasProfile: meta.hasProfile,
    updatedAt: meta.updatedAt,
    bytes: meta.bytes,
    sha256: meta.sha256
  });
}

async function downloadProfile(req, res, playerId) {
  const auth = await authenticate(req, playerId);
  if (!auth.ok) return sendJson(res, auth.statusCode, { ok: false, error: auth.error });
  const meta = auth.meta;
  if (!meta.hasProfile || meta.revision <= 0) {
    return sendJson(res, 404, { ok: false, error: 'profile-save-not-created' });
  }

  const filePath = currentPath(playerId);
  let stat;
  try {
    stat = await fsp.stat(filePath);
  } catch {
    return sendJson(res, 404, { ok: false, error: 'profile-archive-missing' });
  }

  res.writeHead(200, {
    'Content-Type': 'application/zip',
    'Content-Length': String(stat.size),
    'Content-Disposition': `attachment; filename="${playerId}-revision-${meta.revision}.zip"`,
    'X-Profile-Revision': String(meta.revision),
    'X-Profile-Sha256': meta.sha256 || '',
    'Cache-Control': 'no-store'
  });
  fs.createReadStream(filePath).pipe(res);
}

async function receiveUpload(req, tempPath) {
  const hash = crypto.createHash('sha256');
  const file = fs.createWriteStream(tempPath, { flags: 'wx', mode: 0o600 });
  let total = 0;
  let prefix = Buffer.alloc(0);

  try {
    for await (const chunk of req) {
      total += chunk.length;
      if (total > MAX_UPLOAD_BYTES) {
        const error = new Error('profile-upload-too-large');
        error.statusCode = 413;
        throw error;
      }
      if (prefix.length < 4) {
        prefix = Buffer.concat([prefix, chunk]).subarray(0, 4);
      }
      hash.update(chunk);
      if (!file.write(chunk)) {
        await new Promise(resolve => file.once('drain', resolve));
      }
    }
  } finally {
    await new Promise(resolve => file.end(resolve));
  }

  if (total < 22 || prefix[0] !== 0x50 || prefix[1] !== 0x4b) {
    const error = new Error('profile-upload-not-a-zip');
    error.statusCode = 400;
    throw error;
  }

  return { bytes: total, sha256: hash.digest('hex') };
}

async function pruneBackups(playerId) {
  const dir = backupsDir(playerId);
  let files;
  try {
    files = await fsp.readdir(dir, { withFileTypes: true });
  } catch {
    return;
  }
  const candidates = [];
  for (const entry of files) {
    if (!entry.isFile() || !entry.name.endsWith('.zip')) continue;
    const full = path.join(dir, entry.name);
    const stat = await fsp.stat(full);
    candidates.push({ full, mtimeMs: stat.mtimeMs });
  }
  candidates.sort((a, b) => b.mtimeMs - a.mtimeMs);
  for (const candidate of candidates.slice(MAX_BACKUPS)) {
    await fsp.unlink(candidate.full).catch(() => {});
  }
}

async function uploadProfile(req, res, playerId) {
  const auth = await authenticate(req, playerId);
  if (!auth.ok) return sendJson(res, auth.statusCode, { ok: false, error: auth.error });
  const meta = auth.meta;

  const baseRevision = Number.parseInt(req.headers['x-profile-base-revision'] || '0', 10);
  if (!Number.isSafeInteger(baseRevision) || baseRevision < 0) {
    return sendJson(res, 400, { ok: false, error: 'invalid-base-revision' });
  }
  if (baseRevision !== meta.revision) {
    return sendJson(res, 409, {
      ok: false,
      error: 'profile-revision-conflict',
      serverRevision: meta.revision
    });
  }

  const dir = profileDir(playerId);
  await fsp.mkdir(dir, { recursive: true, mode: 0o700 });
  await fsp.mkdir(backupsDir(playerId), { recursive: true, mode: 0o700 });
  const tempPath = path.join(dir, `upload-${process.pid}-${Date.now()}.tmp`);

  let upload;
  try {
    upload = await receiveUpload(req, tempPath);
  } catch (error) {
    await fsp.unlink(tempPath).catch(() => {});
    throw error;
  }

  const oldCurrent = currentPath(playerId);
  if (meta.hasProfile && fs.existsSync(oldCurrent)) {
    const backupName = `revision-${String(meta.revision).padStart(8, '0')}-${Date.now()}.zip`;
    await fsp.copyFile(oldCurrent, path.join(backupsDir(playerId), backupName));
  }

  await fsp.rename(tempPath, oldCurrent);
  const revision = meta.revision + 1;
  const updated = {
    ...meta,
    revision,
    hasProfile: true,
    updatedAt: nowIso(),
    bytes: upload.bytes,
    sha256: upload.sha256,
    launcherVersion: cleanText(req.headers['x-launcher-version'] || meta.launcherVersion, 32)
  };
  await atomicWriteJson(metaPath(playerId), updated);
  await pruneBackups(playerId);

  return sendJson(res, 200, {
    ok: true,
    playerId,
    revision,
    bytes: upload.bytes,
    sha256: upload.sha256,
    updatedAt: updated.updatedAt
  });
}

async function route(req, res) {
  const url = new URL(req.url, `http://${req.headers.host || 'localhost'}`);
  const pathname = url.pathname.replace(/\/+$/, '') || '/';

  if (req.method === 'GET' && (pathname === '/' || pathname === '/health')) {
    return sendJson(res, 200, {
      ok: true,
      service: SERVICE,
      version: VERSION,
      maxUploadBytes: MAX_UPLOAD_BYTES,
      serverTime: nowIso()
    });
  }

  if (req.method === 'POST' && pathname === '/v1/register') {
    return registerProfile(req, res);
  }

  const match = /^\/v1\/profiles\/([A-Za-z0-9_-]{18,128})\/(meta|download|upload)$/.exec(pathname);
  if (match) {
    const playerId = match[1];
    const action = match[2];
    if (req.method === 'GET' && action === 'meta') return sendMeta(req, res, playerId);
    if (req.method === 'GET' && action === 'download') return downloadProfile(req, res, playerId);
    if (req.method === 'PUT' && action === 'upload') return uploadProfile(req, res, playerId);
  }

  return sendJson(res, 404, { ok: false, error: 'route-not-found' });
}

async function main() {
  await fsp.mkdir(PROFILES_DIR, { recursive: true, mode: 0o700 });
  const server = http.createServer((req, res) => {
    route(req, res).catch(error => {
      const statusCode = Number.isInteger(error.statusCode) ? error.statusCode : 500;
      console.error(`[${nowIso()}]`, error);
      if (!res.headersSent) {
        sendJson(res, statusCode, { ok: false, error: statusCode === 500 ? 'internal-server-error' : error.message });
      } else {
        res.destroy();
      }
    });
  });
  server.requestTimeout = 5 * 60 * 1000;
  server.headersTimeout = 30 * 1000;
  server.listen(PORT, HOST, () => {
    console.log(`${SERVICE} ${VERSION} listening on http://${HOST}:${PORT}`);
  });
}

main().catch(error => {
  console.error(error);
  process.exit(1);
});
