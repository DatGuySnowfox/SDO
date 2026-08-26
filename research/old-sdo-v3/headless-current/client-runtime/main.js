import { createServer } from 'node:http';
import { mkdir, rename, rm, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { log } from "../shared/log.js";
import { ClientSession } from "./session.js";
import { ProfileSync } from "./profile-sync.js";
import { requestJoinTicket } from "./ticket.js";
import { UdpGameBridge } from "./udp-game-bridge.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
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
const profileSync = process.env.SDO_PROFILE_URL && process.env.SDO_PROFILE_SAVE_ROOT
    ? new ProfileSync({
        baseUrl: process.env.SDO_PROFILE_URL,
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
const session = new ClientSession({
    gatewayHost: issued.gatewayHost,
    gatewayPort: issued.gatewayPort,
    ticket: issued.ticket,
    worldId: issued.worldId,
    checkpoint: (reason, force) => profileSync?.checkpoint(reason, force),
    requestGameSave,
    gameBridge: new UdpGameBridge({
        listenPort: Number(process.env.SDO_GAME_BRIDGE_PORT ?? 32100),
        gamePort: Number(process.env.SDO_GAME_BRIDGE_LOCAL_PORT ?? 32101)
    })
});
await session.start();
const statusPort = Number(process.env.SDO_CLIENT_STATUS_PORT ?? 32200);
const statusServer = createServer((request, response) => {
    const isLoopback = ['127.0.0.1', '::1', '::ffff:127.0.0.1'].includes(request.socket.remoteAddress ?? '');
    if (request.method === 'POST' &&
        request.url === '/shutdown' &&
        isLoopback) {
        response.writeHead(202).end();
        setImmediate(() => void shutdown());
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
statusServer.listen(statusPort, '127.0.0.1');
let shutdownStarted = false;
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
        await session.stop();
    }
    catch (error) {
        exitCode = 1;
        log('error', 'client_session_shutdown_failed', {
            error: error instanceof Error ? error.message : 'client_session_shutdown_failed'
        });
    }
    await new Promise((resolve) => statusServer.close(() => resolve()));
    process.exit(exitCode);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map