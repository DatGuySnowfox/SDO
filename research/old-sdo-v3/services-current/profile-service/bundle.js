import { createHash } from 'node:crypto';
export const PROFILE_BUNDLE_VERSION = 1;
export const MAX_PROFILE_BYTES = 32 * 1024 * 1024;
export const MAX_PROFILE_FILE_BYTES = 16 * 1024 * 1024;
export const MAX_PROFILE_FILES = 256;
export const PROFILE_PATHS = [
    'SurrounDead.sav',
    'SaveSlot/Difficulties.sav',
    'SaveSlot/General.sav',
    'SaveSlot/Level.sav',
    'SaveSlot/Player.sav',
    'SaveSlot/PlayerInfo.sav',
    'SaveSlot/Slot.sav',
    'SaveSlot/Thumb.jpg'
];
export const CANONICAL_SERVER_PROFILE_PATHS = [
    ...PROFILE_PATHS,
    'SaveSlot_Quests.sav',
    'SaveSlot_SkillTree.sav'
];
const QUICK_SAVE_SLOT_FILE = /^(?:Difficulties|General|Level|Player|PlayerInfo|Slot)\.sav$|^Thumb\.jpg$/;
const QUICK_SAVE_ROOT_FILE = /^(QuickSave[^/]+)_(Quests|SkillTree)\.sav$/;
const QUICK_SAVE_DIRECTORY = /^QuickSave[^/]+$/;
const STEAM_ID = /^steam_(7656119[0-9]{10})$/;
const SLOT_FILE = /^(?:Difficulties|General|Level|Player|PlayerInfo|Slot)\.sav$|^Thumb\.jpg$/;
const SLOT_DIRECTORY = /^[A-Za-z0-9_-]{1,64}$/;
const ROOT_PROGRESS_FILE = /^(?:[A-Za-z0-9_-]{1,64})_(?:Quests|SkillTree)\.sav$/;
export function validSteamIdentity(value) {
    return STEAM_ID.test(value);
}
export function validProfilePath(value) {
    if (value === 'SurrounDead.sav')
        return true;
    const parts = value.split('/');
    if (parts.length === 1)
        return ROOT_PROGRESS_FILE.test(parts[0]);
    return parts.length === 2 &&
        SLOT_DIRECTORY.test(parts[0]) &&
        SLOT_FILE.test(parts[1]);
}
export function sha256(value) {
    return createHash('sha256').update(value).digest('hex');
}
export function validateProfileBundle(value, expectedSteamId) {
    if (!value || typeof value !== 'object' || Array.isArray(value)) {
        throw new Error('profile_bundle_invalid');
    }
    const bundle = value;
    if (bundle.version !== PROFILE_BUNDLE_VERSION ||
        typeof bundle.steamId !== 'string' ||
        !validSteamIdentity(bundle.steamId) ||
        (expectedSteamId !== undefined && bundle.steamId !== expectedSteamId) ||
        !Number.isSafeInteger(bundle.createdAtMs) ||
        Number(bundle.createdAtMs) < 0 ||
        !Array.isArray(bundle.files) ||
        bundle.files.length < 1 ||
        bundle.files.length > MAX_PROFILE_FILES) {
        throw new Error('profile_bundle_invalid');
    }
    let total = 0;
    const seen = new Set();
    const files = bundle.files.map((input) => {
        if (!input || typeof input !== 'object' || Array.isArray(input)) {
            throw new Error('profile_file_invalid');
        }
        const file = input;
        if (typeof file.path !== 'string' ||
            !validProfilePath(file.path) ||
            seen.has(file.path) ||
            !Number.isSafeInteger(file.bytes) ||
            Number(file.bytes) < 1 ||
            Number(file.bytes) > MAX_PROFILE_FILE_BYTES ||
            typeof file.sha256 !== 'string' ||
            !/^[a-f0-9]{64}$/.test(file.sha256) ||
            typeof file.data !== 'string') {
            throw new Error('profile_file_invalid');
        }
        let decoded;
        try {
            decoded = Buffer.from(file.data, 'base64');
        }
        catch {
            throw new Error('profile_file_invalid_base64');
        }
        if (decoded.length !== file.bytes ||
            sha256(decoded) !== file.sha256) {
            throw new Error('profile_file_hash_mismatch');
        }
        total += decoded.length;
        if (total > MAX_PROFILE_BYTES) {
            throw new Error('profile_bundle_too_large');
        }
        seen.add(file.path);
        return {
            path: file.path,
            bytes: decoded.length,
            sha256: file.sha256,
            data: file.data
        };
    });
    return {
        version: PROFILE_BUNDLE_VERSION,
        steamId: bundle.steamId,
        createdAtMs: Number(bundle.createdAtMs),
        files
    };
}
export function canonicalProfileBundle(bundle) {
    return `${JSON.stringify({
        ...bundle,
        files: [...bundle.files].sort((left, right) => left.path.localeCompare(right.path))
    })}\n`;
}
export function profileContentSha256(bundle) {
    return sha256(JSON.stringify([...bundle.files]
        .sort((left, right) => left.path.localeCompare(right.path))
        .map(({ path, bytes, sha256: fileSha256 }) => ({
        path,
        bytes,
        sha256: fileSha256
    }))));
}
export function isCompleteCanonicalServerBundle(bundle) {
    const paths = new Set(bundle.files.map((file) => file.path));
    return PROFILE_PATHS.every((path) => paths.has(path));
}
export function normalizeQuickSaveBundleToSaveSlot(bundle, expectedSteamId) {
    if (isCompleteCanonicalServerBundle(bundle)) {
        return validateProfileBundle(bundle, expectedSteamId);
    }
    const byPath = new Map(bundle.files.map((file) => [file.path, file]));
    const quickSaveDirectories = [...new Set(bundle.files
            .map((file) => file.path.split('/'))
            .filter((parts) => parts.length === 2 && QUICK_SAVE_DIRECTORY.test(parts[0]))
            .map((parts) => parts[0]))].sort((left, right) => right.localeCompare(left));
    const quickSaveDirectory = quickSaveDirectories[0];
    if (quickSaveDirectory) {
        for (const file of bundle.files) {
            const parts = file.path.split('/');
            if (parts.length !== 2 ||
                parts[0] !== quickSaveDirectory ||
                !QUICK_SAVE_SLOT_FILE.test(parts[1]))
                continue;
            byPath.set(`SaveSlot/${parts[1]}`, {
                ...file,
                path: `SaveSlot/${parts[1]}`
            });
        }
        for (const file of bundle.files) {
            const match = QUICK_SAVE_ROOT_FILE.exec(file.path);
            if (!match || match[1] !== quickSaveDirectory)
                continue;
            byPath.set(`SaveSlot_${match[2]}.sav`, {
                ...file,
                path: `SaveSlot_${match[2]}.sav`
            });
        }
    }
    const normalized = validateProfileBundle({
        version: PROFILE_BUNDLE_VERSION,
        steamId: bundle.steamId,
        createdAtMs: bundle.createdAtMs,
        files: [...byPath.values()]
            .filter((file) => file.path === 'SurrounDead.sav' ||
            file.path.startsWith('SaveSlot/') ||
            file.path === 'SaveSlot_Quests.sav' ||
            file.path === 'SaveSlot_SkillTree.sav')
    }, expectedSteamId);
    return normalized;
}
//# sourceMappingURL=bundle.js.map