import { createHash } from 'node:crypto';
import { mkdir, readFile, rename, rm, writeFile } from 'node:fs/promises';
import { join } from 'node:path';
export const MAX_SERVER_ICON_BYTES = 512 * 1024;
const SERVER_ID = /^[a-z0-9][a-z0-9-]{2,47}$/;
export function detectServerIconContentType(buffer) {
    if (buffer.length >= 8 &&
        buffer[0] === 0x89 &&
        buffer[1] === 0x50 &&
        buffer[2] === 0x4e &&
        buffer[3] === 0x47) {
        return 'image/png';
    }
    if (buffer.length >= 3 && buffer[0] === 0xff && buffer[1] === 0xd8 && buffer[2] === 0xff) {
        return 'image/jpeg';
    }
    if (buffer.length >= 12 &&
        buffer.toString('ascii', 0, 4) === 'RIFF' &&
        buffer.toString('ascii', 8, 12) === 'WEBP') {
        return 'image/webp';
    }
    return undefined;
}
function iconExtension(contentType) {
    if (contentType === 'image/jpeg')
        return 'jpg';
    if (contentType === 'image/webp')
        return 'webp';
    return 'png';
}
export function serverIconFileName(serverId, contentType) {
    if (!SERVER_ID.test(serverId))
        throw new Error('server_icon_invalid_id');
    return `${serverId}.${iconExtension(contentType)}`;
}
export async function readServerIcon(root, serverId) {
    if (!SERVER_ID.test(serverId))
        return undefined;
    for (const contentType of ['image/png', 'image/jpeg', 'image/webp']) {
        const path = join(root, serverIconFileName(serverId, contentType));
        try {
            const buffer = await readFile(path);
            const detected = detectServerIconContentType(buffer);
            if (!detected || detected !== contentType)
                continue;
            return { buffer, contentType };
        }
        catch {
            continue;
        }
    }
    return undefined;
}
export async function saveServerIcon(root, serverId, buffer, contentType) {
    if (!SERVER_ID.test(serverId))
        throw new Error('server_icon_invalid_id');
    const detected = detectServerIconContentType(buffer);
    if (!detected || detected !== contentType)
        throw new Error('server_icon_invalid_image');
    if (buffer.length < 32 || buffer.length > MAX_SERVER_ICON_BYTES) {
        throw new Error('server_icon_invalid_size');
    }
    await mkdir(root, { recursive: true });
    for (const candidate of ['image/png', 'image/jpeg', 'image/webp']) {
        if (candidate === contentType)
            continue;
        await rm(join(root, serverIconFileName(serverId, candidate)), { force: true });
    }
    const path = join(root, serverIconFileName(serverId, contentType));
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, buffer, { mode: 0o600 });
    await rename(temporary, path);
    return Date.now();
}
export async function deleteServerIcon(root, serverId) {
    if (!SERVER_ID.test(serverId))
        throw new Error('server_icon_invalid_id');
    await Promise.all(['image/png', 'image/jpeg', 'image/webp'].map((contentType) => rm(join(root, serverIconFileName(serverId, contentType)), { force: true })));
}
export function parseServerIconUpload(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input)) {
        throw new Error('server_icon_invalid_payload');
    }
    const body = input;
    if (typeof body.imageBase64 !== 'string' || body.imageBase64.length < 1) {
        throw new Error('server_icon_invalid_payload');
    }
    if (body.contentType !== 'image/png' &&
        body.contentType !== 'image/jpeg' &&
        body.contentType !== 'image/webp') {
        throw new Error('server_icon_invalid_content_type');
    }
    let buffer;
    try {
        buffer = Buffer.from(body.imageBase64, 'base64');
    }
    catch {
        throw new Error('server_icon_invalid_payload');
    }
    const detected = detectServerIconContentType(buffer);
    if (!detected || detected !== body.contentType) {
        throw new Error('server_icon_invalid_image');
    }
    if (buffer.length < 32 || buffer.length > MAX_SERVER_ICON_BYTES) {
        throw new Error('server_icon_invalid_size');
    }
    return { buffer, contentType: body.contentType };
}
export function buildServerIconUrl(publicBaseUrl, serverId, iconUpdatedAtMs) {
    if (!publicBaseUrl || !iconUpdatedAtMs || iconUpdatedAtMs < 1)
        return undefined;
    const base = publicBaseUrl.replace(/\/$/, '');
    return `${base}/v1/servers/${encodeURIComponent(serverId)}/icon?v=${iconUpdatedAtMs}`;
}
export function serverIconEtag(buffer) {
    return `"${createHash('sha256').update(buffer).digest('hex').slice(0, 16)}"`;
}
//# sourceMappingURL=icons.js.map