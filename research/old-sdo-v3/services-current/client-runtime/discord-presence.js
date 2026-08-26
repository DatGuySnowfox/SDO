import { connect } from 'node:net';
import { appendFile, mkdir, readFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';
const DefaultApplicationId = '1523611604279890021';
const moduleDir = dirname(fileURLToPath(import.meta.url));
let socket = null;
let reader = null;
let connected = false;
let ready = false;
let sessionStart = Math.floor(Date.now() / 1000);
let config = null;
const defaultConfig = () => ({
    applicationId: process.env.SDO_DISCORD_APP_ID?.trim() || DefaultApplicationId,
    largeImageUrl: process.env.SDO_DISCORD_IMAGE_URL?.trim() ||
        'https://sdo.harzgg.space/updates/assets/sdo-hero-v1.png',
    largeImageKey: process.env.SDO_DISCORD_IMAGE_KEY?.trim() || 'survivor',
    largeImageText: process.env.SDO_DISCORD_IMAGE_TEXT?.trim() || 'Surviving the Outbreak',
    smallImageUrl: process.env.SDO_DISCORD_SMALL_IMAGE_URL?.trim() ||
        'https://sdo.harzgg.space/updates/assets/sdo-online-logo.png',
    smallImageKey: process.env.SDO_DISCORD_SMALL_IMAGE_KEY?.trim() || 'sdo_logo',
    smallImageText: process.env.SDO_DISCORD_SMALL_IMAGE_TEXT?.trim() || 'SD-Online',
    githubUrl: process.env.SDO_DISCORD_GITHUB_URL?.trim() || 'https://github.com/harzgg/SD-Online/releases',
    discordInviteUrl: process.env.SDO_DISCORD_INVITE_URL?.trim() || 'https://discord.com/invite/RCKNT9Ajg4',
    buttons: [
        { label: 'GitHub', url: 'https://github.com/harzgg/SD-Online/releases' },
        { label: 'Discord', url: 'https://discord.com/invite/RCKNT9Ajg4' }
    ]
});
async function logDebug(message) {
    if (process.env.SDO_DISCORD_PRESENCE_DEBUG !== '1')
        return;
    const root = join(process.env.LOCALAPPDATA ?? process.env.HOME ?? '.', 'SurrounDeadOnline', 'ServerBrowser', 'Logs');
    await mkdir(root, { recursive: true });
    await appendFile(join(root, 'discord-presence.log'), `${new Date().toISOString()} ${message}\n`);
}
async function loadConfig() {
    if (config)
        return config;
    const merged = defaultConfig();
    const candidates = [
        join(moduleDir, 'discord-presence.config.json'),
        join(process.cwd(), 'discord-presence.config.json')
    ];
    for (const path of candidates) {
        try {
            const parsed = JSON.parse(await readFile(path, 'utf8'));
            if (parsed.applicationId?.trim())
                merged.applicationId = parsed.applicationId.trim();
            if (parsed.largeImageUrl?.trim())
                merged.largeImageUrl = parsed.largeImageUrl.trim();
            if (parsed.largeImageKey?.trim())
                merged.largeImageKey = parsed.largeImageKey.trim();
            if (parsed.largeImageText?.trim())
                merged.largeImageText = parsed.largeImageText.trim();
            if (parsed.smallImageUrl?.trim())
                merged.smallImageUrl = parsed.smallImageUrl.trim();
            if (parsed.smallImageKey?.trim())
                merged.smallImageKey = parsed.smallImageKey.trim();
            if (parsed.smallImageText?.trim())
                merged.smallImageText = parsed.smallImageText.trim();
            if (parsed.githubUrl?.trim())
                merged.githubUrl = parsed.githubUrl.trim();
            if (parsed.discordInviteUrl?.trim())
                merged.discordInviteUrl = parsed.discordInviteUrl.trim();
            if (Array.isArray(parsed.buttons) && parsed.buttons.length > 0) {
                merged.buttons = parsed.buttons
                    .filter((entry) => Boolean(entry?.label?.trim()) && Boolean(entry?.url?.trim()))
                    .map((entry) => ({ label: entry.label.trim(), url: entry.url.trim() }));
            }
            break;
        }
        catch {
            // optional config
        }
    }
    config = merged;
    return merged;
}
function writeFrame(opcode, payload) {
    if (!socket)
        return;
    const body = Buffer.from(payload, 'utf8');
    const header = Buffer.alloc(8);
    header.writeInt32LE(opcode, 0);
    header.writeInt32LE(body.length, 4);
    socket.write(header);
    socket.write(body);
}
async function readFrame(timeoutMs = 3000) {
    if (!socket)
        return null;
    const header = await readBytes(8, timeoutMs);
    if (!header || header.length < 8)
        return null;
    const opcode = header.readInt32LE(0);
    const length = header.readInt32LE(4);
    if (length < 0 || length > 1_048_576)
        return null;
    const body = length === 0 ? Buffer.alloc(0) : await readBytes(length, timeoutMs);
    if (!body || body.length !== length)
        return null;
    return { opcode, json: body.toString('utf8') };
}
function readBytes(length, timeoutMs) {
    const sock = socket;
    if (!sock)
        return Promise.resolve(null);
    return new Promise((resolve) => {
        const chunks = [];
        let total = 0;
        const timer = setTimeout(() => {
            cleanup();
            resolve(null);
        }, timeoutMs);
        const onData = (chunk) => {
            chunks.push(chunk);
            total += chunk.length;
            if (total >= length) {
                cleanup();
                resolve(Buffer.concat(chunks).subarray(0, length));
            }
        };
        const onError = () => {
            cleanup();
            resolve(null);
        };
        const cleanup = () => {
            clearTimeout(timer);
            sock.off('data', onData);
            sock.off('error', onError);
            sock.off('end', onError);
        };
        sock.on('data', onData);
        sock.once('error', onError);
        sock.once('end', onError);
    });
}
async function readLoop() {
    if (!socket)
        return;
    while (connected && !socket.destroyed) {
        const frame = await readFrame(60000);
        if (!frame)
            break;
        if (frame.opcode === 3)
            writeFrame(4, frame.json);
    }
    connected = false;
    ready = false;
}
async function waitForReady() {
    const deadline = Date.now() + 3000;
    while (Date.now() < deadline) {
        const frame = await readFrame(Math.max(250, deadline - Date.now()));
        if (!frame)
            continue;
        if (frame.opcode === 3) {
            writeFrame(4, frame.json);
            continue;
        }
        if (frame.opcode !== 1)
            continue;
        if (frame.json.includes('READY'))
            return true;
        if (frame.json.includes('ERROR')) {
            await logDebug(`discord_error: ${frame.json}`);
            return false;
        }
    }
    return false;
}
async function connectDiscord(appId) {
    if (connected && ready && socket)
        return true;
    if (socket) {
        connected = false;
        ready = false;
        socket.destroy();
        socket = null;
    }
    for (let index = 0; index < 10; index += 1) {
        const path = `\\\\.\\pipe\\discord-ipc-${index}`;
        try {
            const client = connect(path);
            await new Promise((resolve, reject) => {
                client.once('connect', () => resolve());
                client.once('error', reject);
            });
            socket = client;
            connected = true;
            ready = false;
            sessionStart = Math.floor(Date.now() / 1000);
            writeFrame(0, JSON.stringify({ v: 1, client_id: appId }));
            if (!(await waitForReady())) {
                await logDebug(`ready_timeout on ${path}`);
                client.destroy();
                socket = null;
                connected = false;
                continue;
            }
            ready = true;
            await logDebug(`connected on ${path}`);
            return true;
        }
        catch (error) {
            await logDebug(`connect_failed ${path}: ${error instanceof Error ? error.message : 'connect_failed'}`);
        }
    }
    return false;
}
function buildButtons(cfg) {
    const buttons = [];
    for (const entry of cfg.buttons) {
        if (buttons.length >= 2)
            break;
        const url = entry.url.trim();
        if (!url.startsWith('http://') && !url.startsWith('https://'))
            continue;
        const label = entry.label.trim().slice(0, 32);
        if (!label)
            continue;
        buttons.push({ label, url });
    }
    if (buttons.length === 0) {
        return [
            { label: 'GitHub', url: cfg.githubUrl },
            { label: 'Discord', url: cfg.discordInviteUrl }
        ];
    }
    return buttons;
}
function buildActivity(details, state, cfg) {
    const largeImage = cfg.largeImageUrl.trim() || cfg.largeImageKey;
    const smallImage = cfg.smallImageUrl.trim() || cfg.smallImageKey;
    const assets = {
        large_image: largeImage,
        large_text: cfg.largeImageText
    };
    if (smallImage) {
        assets.small_image = smallImage;
        assets.small_text = cfg.smallImageText;
    }
    return {
        type: 0,
        details: details.slice(0, 128),
        state: state.slice(0, 128),
        timestamps: { start: sessionStart },
        assets,
        buttons: buildButtons(cfg)
    };
}
async function setActivity(details, state) {
    const cfg = await loadConfig();
    if (!cfg.applicationId)
        return;
    if (!(await connectDiscord(cfg.applicationId)))
        return;
    writeFrame(1, JSON.stringify({
        cmd: 'SET_ACTIVITY',
        args: {
            pid: process.pid,
            activity: buildActivity(details, state, cfg)
        },
        nonce: `${Date.now()}-${process.pid}`
    }));
    if (!reader) {
        reader = readLoop().catch(() => {
            connected = false;
            ready = false;
        });
    }
    await logDebug(`set_activity details=${details} state=${state}`);
}
export async function startLauncherPresence() {
    await setActivity('Open World PvE Survival', 'In Launcher');
}
export async function startWorldPresence(worldName) {
    const safeName = worldName.trim() || 'SurrounDead Online';
    await setActivity(safeName, 'SurrounDead Online');
}
export async function clearDiscordPresence() {
    const cfg = await loadConfig();
    if (!cfg.applicationId || !connected || !ready || !socket)
        return;
    writeFrame(1, JSON.stringify({
        cmd: 'SET_ACTIVITY',
        args: { pid: process.pid, activity: null },
        nonce: `${Date.now()}-${process.pid}-clear`
    }));
    connected = false;
    ready = false;
    socket.end();
    socket = null;
    await reader;
    reader = null;
}
//# sourceMappingURL=discord-presence.js.map