import { writeSync } from 'node:fs';
import { createServer } from 'node:http';
import { mkdir, rename, rm, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { ensureUnbufferedOutput, log } from "../shared/log.js";
ensureUnbufferedOutput();
writeSync(2, `client_runtime_boot pid=${process.pid}\n`);
import { ClientSession } from "./session.js";
import { ProfileSync } from "./profile-sync.js";
import { requestJoinTicket } from "./ticket.js";
import { UdpGameBridge } from "./udp-game-bridge.js";
import { clearDiscordPresence, startWorldPresence } from "./discord-presence.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const parseGatewayPorts = () => {
    const raw = process.env.SDO_GATEWAY_PORTS?.trim();
    if (!raw)
        return undefined;
    const ports = raw.split(/[,\s]+/).map((part) => Number(part.trim()));
    const valid = ports.filter((port) => Number.isInteger(port) && port > 0 && port <= 65_535);
    return valid.length > 0 ? valid : undefined;
};
const parseGatewayEndpoints = () => {
    const raw = process.env.SDO_GATEWAY_ENDPOINTS?.trim();
    if (!raw)
        return undefined;
    const endpoints = [];
    for (const part of raw.split(/[,;]+/)) {
        const trimmed = part.trim();
        if (!trimmed)
            continue;
        const colon = trimmed.lastIndexOf(':');
        if (colon <= 0)
            continue;
        const host = trimmed.slice(0, colon).trim();
        const port = Number(trimmed.slice(colon + 1).trim());
        if (!host || !Number.isInteger(port) || port < 1 || port > 65_535)
            continue;
        endpoints.push({ host, port });
    }
    return endpoints.length > 0 ? endpoints : undefined;
};
const parseProfileEndpoints = () => {
    const raw = process.env.SDO_PROFILE_ENDPOINTS?.trim();
    if (!raw)
        return undefined;
    const urls = raw.split(/[,;]+/).map((part) => part.trim().replace(/\/+$/, ''));
    const valid = urls.filter(Boolean);
    return valid.length > 0 ? [...new Set(valid)] : undefined;
};
const listenStatusServer = (server, port) => new Promise((resolve, reject) => {
    server.once('error', reject);
    server.listen(port, '127.0.0.1', () => {
        server.off('error', reject);
        resolve();
    });
});
let shutdownStarted = false;
let statusServer;
let session;
let profileSync;
const shutdown = async () => {
    if (shutdownStarted)
        return;
    shutdownStarted = true;
    let exitCode = 0;
    try {
        if (profileSync)
            await profileSync.stop();
    }
    catch (error) {
        exitCode = 1;
        log('error', 'profile_final_checkpoint_failed', {
            error: error instanceof Error ? error.message : 'profile_final_checkpoint_failed'
        });
    }
    try {
        if (session)
            await session.stop();
    }
    catch (error) {
        exitCode = 1;
        log('error', 'client_session_shutdown_failed', {
            error: error instanceof Error ? error.message : 'client_session_shutdown_failed'
        });
    }
    try {
        await clearDiscordPresence();
    }
    catch {
        // best effort
    }
    if (statusServer) {
        await new Promise((resolve) => statusServer.close(() => resolve()));
    }
    process.exit(exitCode);
};
const fatal = (event, error) => {
    const message = error instanceof Error ? error.message : String(error);
    log('error', event, { error: message });
    writeSync(2, `${event}: ${message}\n`);
    process.exit(1);
};
process.on('SIGINT', () => void shutdown());
process.on('SIGTERM', () => void shutdown());
process.on('uncaughtException', (error) => {
    fatal('client_runtime_uncaught_exception', error);
});
process.on('unhandledRejection', (reason) => {
    fatal('client_runtime_unhandled_rejection', reason);
});
async function bootstrap() {
    log('info', 'client_runtime_starting', {
        statusPort: Number(process.env.SDO_CLIENT_STATUS_PORT ?? 32200),
        worldId: process.env.SDO_WORLD_ID ?? 'unknown'
    });
    const issued = process.env.SDO_JOIN_TICKET
        ? {
            ticket: process.env.SDO_JOIN_TICKET,
            gatewayHost: process.env.SDO_GATEWAY_HOST ?? '127.0.0.1',
            gatewayPort: Number(process.env.SDO_GATEWAY_PORT ?? 31000),
            worldId: process.env.SDO_WORLD_ID ?? '11111111-1111-4111-8111-111111111111'
        }
        : await requestJoinTicket({
            ticketUrl: required('SDO_TICKET_URL'),
            playerId: required('SDO_PLAYER_ID'),
            displayName: required('SDO_DISPLAY_NAME'),
            identityToken: required('SDO_IDENTITY_TOKEN')
        });
    profileSync = process.env.SDO_PROFILE_URL && process.env.SDO_PROFILE_SAVE_ROOT
        ? new ProfileSync({
            baseUrl: process.env.SDO_PROFILE_URL,
            baseUrls: parseProfileEndpoints(),
            joinTicket: issued.ticket,
            saveRoot: process.env.SDO_PROFILE_SAVE_ROOT,
            checkpointMs: Number(process.env.SDO_PROFILE_CHECKPOINT_MS ?? 5_000),
            serverCanonical: process.env.SDO_PROFILE_SERVER_CANONICAL === '1'
        })
        : undefined;
    if (profileSync)
        await profileSync.start();
    const gameDataRoot = process.env.SDO_GAME_DATA_DIR;
    const requestGameSave = gameDataRoot
        ? async (reason, delayMs) => {
            const destination = join(gameDataRoot, 'server_profile_save_request.tsv');
            const temporary = `${destination}.${process.pid}.${Date.now()}.tmp`;
            const safeReason = reason.replace(/[^A-Za-z0-9_-]/g, '_').slice(0, 64);
            await mkdir(dirname(destination), { recursive: true });
            await writeFile(temporary, `${Date.now() + Math.max(0, Math.min(delayMs, 30_000))}\t${safeReason}\n`, { mode: 0o600 });
            await rm(destination, { force: true });
            await rename(temporary, destination);
        }
        : undefined;
    session = new ClientSession({
        gatewayHost: issued.gatewayHost,
        gatewayPort: issued.gatewayPort,
        gatewayPorts: parseGatewayPorts(),
        gatewayEndpoints: parseGatewayEndpoints(),
        ticket: issued.ticket,
        worldId: issued.worldId,
        checkpoint: (reason, force) => profileSync?.checkpoint(reason, force),
        requestGameSave,
        gameBridge: new UdpGameBridge({
            listenPort: Number(process.env.SDO_GAME_BRIDGE_PORT ?? 32100),
            gamePort: Number(process.env.SDO_GAME_BRIDGE_LOCAL_PORT ?? 32101)
        })
    });
    const statusPort = Number(process.env.SDO_CLIENT_STATUS_PORT ?? 32200);
    statusServer = createServer((request, response) => {
        const isLoopback = ['127.0.0.1', '::1', '::ffff:127.0.0.1'].includes(request.socket.remoteAddress ?? '');
        if (request.method === 'POST' &&
            request.url === '/shutdown' &&
            isLoopback) {
            response.writeHead(202).end();
            setImmediate(() => void shutdown());
            return;
        }
        if (request.method === 'POST' &&
            request.url === '/recover-profile' &&
            isLoopback &&
            profileSync) {
            void profileSync.recoverPrevious().then((result) => {
                const body = JSON.stringify({ ok: true, ...result });
                response.writeHead(200, {
                    'content-type': 'application/json',
                    'content-length': Buffer.byteLength(body),
                    'cache-control': 'no-store'
                });
                response.end(body);
            }, (error) => {
                const body = JSON.stringify({
                    ok: false,
                    error: error instanceof Error ? error.message : 'profile_recovery_failed'
                });
                response.writeHead(409, {
                    'content-type': 'application/json',
                    'content-length': Buffer.byteLength(body),
                    'cache-control': 'no-store'
                });
                response.end(body);
            });
            return;
        }
        if (request.method === 'POST' &&
            request.url?.startsWith('/checkpoint') &&
            isLoopback &&
            profileSync) {
            const url = new URL(request.url, `http://127.0.0.1:${statusPort}`);
            const reason = (url.searchParams.get('reason') ?? 'game-save-confirmed')
                .replace(/[^A-Za-z0-9_-]/g, '_')
                .slice(0, 64);
            void profileSync.checkpoint(reason, true).then(() => response.writeHead(200).end(), () => response.writeHead(500).end());
            return;
        }
        if (request.url !== '/status')
            return void response.writeHead(404).end();
        const body = JSON.stringify({
            ...session.status(),
            profile: profileSync?.status() ?? { enabled: false }
        });
        response.writeHead(200, {
            'content-type': 'application/json',
            'content-length': Buffer.byteLength(body),
            'cache-control': 'no-store'
        });
        response.end(body);
    });
    try {
        await listenStatusServer(statusServer, statusPort);
    }
    catch (error) {
        const code = error && typeof error === 'object' && 'code' in error
            ? String(error.code)
            : '';
        throw new Error(code === 'EADDRINUSE'
            ? `status_port_busy_${statusPort}`
            : `status_server_listen_failed_${statusPort}`);
    }
    log('info', 'client_runtime_ready', { statusPort });
    await session.start();
    const worldName = process.env.SDO_SERVER_NAME?.trim();
    if (worldName) {
        void startWorldPresence(worldName).catch((error) => {
            log('warning', 'discord_presence_start_failed', {
                error: error instanceof Error ? error.message : 'discord_presence_start_failed'
            });
        });
    }
}
void bootstrap().catch((error) => {
    fatal('client_runtime_fatal', error);
});
//# sourceMappingURL=main.js.map