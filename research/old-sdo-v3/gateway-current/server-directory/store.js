import { randomBytes, scryptSync, timingSafeEqual } from 'node:crypto';
import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
const ID = /^[a-z0-9][a-z0-9-]{2,47}$/;
const HOST = /^[A-Za-z0-9.:-]{1,255}$/;
const WORLD = /^[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$/i;
export function validateRegistry(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input))
        throw new Error('registry_invalid');
    const value = input;
    if (value.version !== 1 || !Array.isArray(value.servers) || value.servers.length > 100) {
        throw new Error('registry_invalid');
    }
    const ids = new Set();
    const servers = value.servers.map((server) => {
        if (!server ||
            !ID.test(server.id) ||
            ids.has(server.id) ||
            typeof server.name !== 'string' ||
            server.name.length < 1 ||
            server.name.length > 64 ||
            typeof server.description !== 'string' ||
            server.description.length > 200 ||
            !['public', 'private'].includes(server.visibility) ||
            !HOST.test(server.gatewayHost) ||
            !Number.isSafeInteger(server.gatewayPort) ||
            server.gatewayPort < 1 ||
            server.gatewayPort > 65535 ||
            !isHttpUrl(server.healthUrl) ||
            !isHttpUrl(server.profileUrl) ||
            !WORLD.test(server.worldId) ||
            typeof server.ticketSecret !== 'string' ||
            server.ticketSecret.length < 16 ||
            (server.passwordHash !== undefined && !validPasswordHash(server.passwordHash)) ||
            (server.visibility === 'private' && !server.passwordHash) ||
            (server.maxPlayers !== undefined &&
                (!Number.isSafeInteger(server.maxPlayers) || server.maxPlayers < 1 || server.maxPlayers > 256))) {
            throw new Error(`server_invalid_${server?.id ?? 'unknown'}`);
        }
        ids.add(server.id);
        return { ...server };
    });
    return { version: 1, servers };
}
export async function loadRegistry(path) {
    const text = await readFile(path, 'utf8');
    return validateRegistry(JSON.parse(text.replace(/^\uFEFF/, '')));
}
export async function saveRegistry(path, registry) {
    const validated = validateRegistry(registry);
    await mkdir(dirname(path), { recursive: true });
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, `${JSON.stringify(validated, null, 2)}\n`, {
        encoding: 'utf8',
        mode: 0o600
    });
    await rename(temporary, path);
}
export function hashPassword(password) {
    if (password.length < 8 || password.length > 128)
        throw new Error('password_length_invalid');
    const salt = randomBytes(16);
    const cost = 16_384;
    const blockSize = 8;
    const parallelization = 1;
    const digest = scryptSync(password, salt, 32, { N: cost, r: blockSize, p: parallelization });
    return [
        'scrypt',
        cost,
        blockSize,
        parallelization,
        salt.toString('base64url'),
        digest.toString('base64url')
    ].join('$');
}
export function verifyPassword(password, encoded) {
    if (password.length > 128 || !validPasswordHash(encoded))
        return false;
    const [, costText, blockText, parallelText, saltText, digestText] = encoded.split('$');
    const expected = Buffer.from(digestText, 'base64url');
    const supplied = scryptSync(password, Buffer.from(saltText, 'base64url'), expected.length, {
        N: Number(costText),
        r: Number(blockText),
        p: Number(parallelText)
    });
    return supplied.length === expected.length && timingSafeEqual(supplied, expected);
}
function validPasswordHash(value) {
    return /^scrypt\$16384\$8\$1\$[A-Za-z0-9_-]{20,32}\$[A-Za-z0-9_-]{40,48}$/.test(value);
}
function isHttpUrl(value) {
    if (typeof value !== 'string' || value.length > 512)
        return false;
    try {
        return ['http:', 'https:'].includes(new URL(value).protocol);
    }
    catch {
        return false;
    }
}
//# sourceMappingURL=store.js.map