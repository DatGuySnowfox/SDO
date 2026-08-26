'use strict';

// SDO server directory — a minimal, free-tier Cloudflare Worker replacement
// for the old production system's server-directory service (see
// research/old-sdo-v3/services-current/server-directory/). Deliberately much
// smaller: no pre-registered server list, no Steam ownership/OpenID, no
// trials, no mod registry, no icons — just "which home-hosted servers are
// currently up and how do I reach one." Each server self-registers with a
// heartbeat; KV's native per-key TTL handles expiry, so there's no separate
// cleanup job (unlike the old file-backed home-heartbeats.js, which needed
// its own freshness check on every read).
//
// KV value + metadata are both set to the same JSON blob on write so GET
// /v1/servers can read everything back from a single list() call (metadata
// comes back with the key listing) instead of one get() per server.

const HEARTBEAT_TTL_SECONDS = 90; // must stay above host-agent's send interval
const SERVER_ID_RE = /^[a-z0-9-]{6,64}$/;
const HOST_RE = /^[a-zA-Z0-9.-]{1,253}$/;

const CORS_HEADERS = {
    'access-control-allow-origin': '*',
    'access-control-allow-methods': 'GET, POST, OPTIONS',
    'access-control-allow-headers': 'content-type, x-directory-key',
};

function json(body, status = 200) {
    return new Response(JSON.stringify(body), {
        status,
        headers: { 'content-type': 'application/json', ...CORS_HEADERS },
    });
}

function clampInt(value, min, max, fallback) {
    const n = Number.parseInt(value, 10);
    if (!Number.isFinite(n)) return fallback;
    return Math.min(max, Math.max(min, n));
}

async function handleHeartbeat(request, env) {
    const key = request.headers.get('x-directory-key') || '';
    // Timing-safe-ish enough for this threat model (a low-value discovery
    // list, not an auth system with real secrets behind it) — a plain ===
    // is fine here, no need for crypto.subtle.timingSafeEqual.
    if (!env.DIRECTORY_KEY || key !== env.DIRECTORY_KEY) {
        return json({ ok: false, error: 'unauthorized' }, 401);
    }

    let body;
    try {
        body = await request.json();
    } catch {
        return json({ ok: false, error: 'invalid_json' }, 400);
    }

    const serverId = typeof body.serverId === 'string' ? body.serverId.trim() : '';
    const name = typeof body.name === 'string' ? body.name.trim().slice(0, 64) : '';
    const host = typeof body.host === 'string' ? body.host.trim() : '';
    const port = Number.parseInt(body.port, 10);
    // playerCount/maxPlayers are just display stats — clamping a garbage
    // value is fine. port is a connection instruction, not a stat — an
    // out-of-range one must be rejected outright, not silently clamped to
    // the nearest valid boundary (which would point players at the wrong
    // port instead of just failing loudly).
    const playerCount = clampInt(body.playerCount, 0, 999, 0);
    const maxPlayers = clampInt(body.maxPlayers, 1, 999, 32);

    if (!SERVER_ID_RE.test(serverId)) return json({ ok: false, error: 'server_id_invalid' }, 400);
    if (!name) return json({ ok: false, error: 'name_required' }, 400);
    if (!HOST_RE.test(host)) return json({ ok: false, error: 'host_invalid' }, 400);
    if (!Number.isInteger(port) || port < 1 || port > 65535) return json({ ok: false, error: 'port_invalid' }, 400);

    const record = { serverId, name, host, port, playerCount, maxPlayers, lastSeenMs: Date.now() };
    await env.SERVERS.put(`server:${serverId}`, JSON.stringify(record), {
        expirationTtl: HEARTBEAT_TTL_SECONDS,
        metadata: record,
    });

    return json({ ok: true, serverId, heartbeatAcceptedAtMs: record.lastSeenMs });
}

async function listServers(env) {
    const servers = [];
    let cursor;
    do {
        const page = await env.SERVERS.list({ prefix: 'server:', cursor });
        for (const entry of page.keys) {
            if (entry.metadata) servers.push(entry.metadata);
        }
        cursor = page.list_complete ? undefined : page.cursor;
    } while (cursor);

    servers.sort((a, b) => a.name.localeCompare(b.name));
    return servers;
}

async function handleList(env) {
    return json({ ok: true, servers: await listServers(env) });
}

// Proxies a ticket request to the chosen server's own HTTP API. Needed
// because the status page is always HTTPS (Cloudflare Workers) while a
// home-hosted gateway's ticket endpoint is plain HTTP — browsers block that
// combination outright as mixed content. A Worker's outbound fetch is a
// server-to-server request, not subject to that browser restriction, so
// proxying through here is what actually makes the browser-based "Join"
// button work at all, not just a tidiness choice. Also keeps the
// gatewayPort+1-for-HTTP convention (see scripts/join.ps1) out of the
// browser entirely.
async function handleJoin(request, env) {
    let body;
    try {
        body = await request.json();
    } catch {
        return json({ ok: false, error: 'invalid_json' }, 400);
    }

    const serverId = typeof body.serverId === 'string' ? body.serverId.trim() : '';
    const playerId = typeof body.playerId === 'string' ? body.playerId.trim() : '';
    const displayName = typeof body.displayName === 'string' ? body.displayName.trim().slice(0, 32) : 'Player';
    if (!SERVER_ID_RE.test(serverId)) return json({ ok: false, error: 'server_id_invalid' }, 400);
    if (!playerId) return json({ ok: false, error: 'player_id_required' }, 400);

    const raw = await env.SERVERS.get(`server:${serverId}`);
    if (!raw) return json({ ok: false, error: 'server_not_found' }, 404);
    const server = JSON.parse(raw);

    let ticketRes;
    try {
        ticketRes = await fetch(`http://${server.host}:${server.port + 1}/v1/tickets`, {
            method: 'POST',
            headers: { 'content-type': 'application/json' },
            body: JSON.stringify({ playerId, displayName }),
        });
    } catch (e) {
        return json({ ok: false, error: 'server_unreachable', detail: e.message }, 502);
    }
    if (!ticketRes.ok) {
        return json({ ok: false, error: 'ticket_rejected', status: ticketRes.status }, 502);
    }

    const ticketBody = await ticketRes.json();
    return json({ ok: true, host: server.host, port: server.port, ticket: ticketBody.ticket });
}

const STATUS_PAGE_TEMPLATE = `<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>SDO servers</title>
<style>
  :root { color-scheme: light dark; }
  body { font: 15px/1.5 -apple-system, Segoe UI, sans-serif; max-width: 640px; margin: 3rem auto; padding: 0 1.25rem; }
  h1 { font-size: 1.4rem; }
  code, pre { background: rgba(127,127,127,0.15); border-radius: 6px; padding: 0.15rem 0.4rem; }
  pre { padding: 0.8rem 1rem; overflow-x: auto; }
  .server { border: 1px solid rgba(127,127,127,0.3); border-radius: 8px; padding: 0.9rem 1.1rem; margin: 0.7rem 0; }
  .server .name { font-weight: 600; }
  .server .meta { opacity: 0.7; font-size: 0.9rem; }
  .empty { opacity: 0.7; }
  button { font: inherit; padding: 0.3rem 0.7rem; border-radius: 6px; border: 1px solid rgba(127,127,127,0.4); background: transparent; cursor: pointer; }
</style>
</head>
<body>
<h1>SurrounDead servers</h1>

<h2>Currently up</h2>
<div id="servers" class="empty">Loading…</div>

<script>
// Persisted per-browser identity, same role as the PowerShell scripts'
// %APPDATA%\SurrounDeadBridge\player.id file — generated once, reused on
// every future visit so a returning tester keeps the same in-game identity/
// progress instead of getting a fresh one every join.
function getPlayerId() {
    let id = localStorage.getItem('sdbPlayerId');
    if (!id) {
        id = crypto.randomUUID().replace(/-/g, '');
        localStorage.setItem('sdbPlayerId', id);
    }
    return id;
}

let servers = [];

async function refresh() {
    const el = document.getElementById('servers');
    try {
        const res = await fetch('/v1/servers');
        const data = await res.json();
        servers = data.ok ? data.servers : [];
        if (servers.length === 0) {
            el.className = 'empty';
            el.textContent = 'No servers currently up.';
            return;
        }
        el.className = '';
        el.innerHTML = servers.map((s, i) =>
            '<div class="server"><div class="name">' + s.name +
            '</div><div class="meta">' + s.playerCount + '/' + s.maxPlayers + ' players</div>' +
            '<button onclick="joinServer(' + i + ', this)">Join</button></div>'
        ).join('');
    } catch (e) {
        el.className = 'empty';
        el.textContent = 'Could not reach the directory.';
    }
}

// Fetches a ticket via the Worker's own /v1/join proxy (see handleJoin —
// needed because this page is HTTPS and a home-hosted gateway's ticket API
// is plain HTTP, which browsers block outright as mixed content if called
// directly), then hands off to Steam with the ticket as a launch argument —
// steam://run/<appid>//-sdb_host=... -sdb_port=... -sdb_ticket=... — which
// the mod reads via its own command line on startup. No local script, no
// download, no OS security prompt: just this one click.
async function joinServer(index, button) {
    const server = servers[index];
    button.disabled = true;
    button.textContent = 'Joining…';
    try {
        const res = await fetch('/v1/join', {
            method: 'POST',
            headers: { 'content-type': 'application/json' },
            body: JSON.stringify({ serverId: server.serverId, playerId: getPlayerId(), displayName: 'Player' }),
        });
        const data = await res.json();
        if (!data.ok) throw new Error(data.error || 'join failed');
        const args = '-sdb_host=' + data.host + ' -sdb_port=' + data.port + ' -sdb_ticket=' + data.ticket;
        window.location.href = 'steam://run/1645820//' + encodeURIComponent(args);
        button.textContent = 'Launching…';
    } catch (e) {
        button.disabled = false;
        button.textContent = 'Join';
        alert('Could not join: ' + e.message);
    }
}

refresh();
setInterval(refresh, 15000);
</script>
</body>
</html>
`;

function handleStatusPage() {
    return new Response(STATUS_PAGE_TEMPLATE, {
        headers: { 'content-type': 'text/html; charset=utf-8', ...CORS_HEADERS },
    });
}

export default {
    async fetch(request, env) {
        const url = new URL(request.url);

        if (request.method === 'OPTIONS') {
            return new Response(null, { status: 204, headers: CORS_HEADERS });
        }
        if (request.method === 'POST' && url.pathname === '/v1/heartbeat') {
            return handleHeartbeat(request, env);
        }
        if (request.method === 'GET' && url.pathname === '/v1/servers') {
            return handleList(env);
        }
        if (request.method === 'POST' && url.pathname === '/v1/join') {
            return handleJoin(request, env);
        }
        if (request.method === 'GET' && url.pathname === '/') {
            return handleStatusPage();
        }
        return json({ ok: false, error: 'not_found' }, 404);
    },
};
