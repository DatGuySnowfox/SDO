import { mkdir, readFile, rename, rm, writeFile, copyFile, readdir } from 'node:fs/promises';
import { join } from 'node:path';
import { canonicalProfileBundle, profileContentSha256, validateProfileBundle } from "./bundle.js";
export class ProfileConflictError extends Error {
    revision;
    constructor(revision) {
        super('profile_revision_conflict');
        this.revision = revision;
    }
}
export class PlayerProfileStore {
    root;
    keepBackups;
    constructor(root, keepBackups = 5) {
        this.root = root;
        this.keepBackups = keepBackups;
    }
    paths(steamId) {
        const root = join(this.root, steamId);
        return {
            root,
            bundle: join(root, 'profile.json'),
            meta: join(root, 'meta.json'),
            backups: join(root, 'backups')
        };
    }
    async load(steamId) {
        const paths = this.paths(steamId);
        let metaRaw;
        let bundleRaw;
        try {
            [metaRaw, bundleRaw] = await Promise.all([
                readFile(paths.meta, 'utf8'),
                readFile(paths.bundle, 'utf8')
            ]);
        }
        catch (error) {
            if (error.code === 'ENOENT')
                return undefined;
            throw error;
        }
        const meta = JSON.parse(metaRaw);
        const bundle = validateProfileBundle(JSON.parse(bundleRaw), steamId);
        const canonical = canonicalProfileBundle(bundle);
        if (meta.version !== 1 ||
            meta.steamId !== steamId ||
            !Number.isSafeInteger(meta.revision) ||
            Number(meta.revision) < 1 ||
            typeof meta.sha256 !== 'string' ||
            meta.sha256 !== profileContentSha256(bundle) ||
            typeof meta.savedAtMs !== 'number' ||
            typeof meta.idempotencyKey !== 'string') {
            throw new Error('stored_profile_invalid');
        }
        return {
            steamId,
            revision: Number(meta.revision),
            sha256: meta.sha256,
            savedAtMs: meta.savedAtMs,
            idempotencyKey: meta.idempotencyKey,
            bundle
        };
    }
    async save(steamId, expectedRevision, idempotencyKey, input) {
        const bundle = validateProfileBundle(input, steamId);
        const canonical = canonicalProfileBundle(bundle);
        const digest = profileContentSha256(bundle);
        const previous = await this.load(steamId);
        if (previous?.idempotencyKey === idempotencyKey)
            return previous;
        if (previous?.sha256 === digest)
            return previous;
        const currentRevision = previous?.revision ?? 0;
        if (expectedRevision !== currentRevision) {
            throw new ProfileConflictError(currentRevision);
        }
        const paths = this.paths(steamId);
        await mkdir(paths.root, { recursive: true });
        const revision = currentRevision + 1;
        const savedAtMs = Date.now();
        const meta = {
            version: 1,
            steamId,
            revision,
            sha256: digest,
            savedAtMs,
            idempotencyKey
        };
        const suffix = `${process.pid}-${Date.now()}`;
        const bundleTemp = `${paths.bundle}.${suffix}.tmp`;
        const metaTemp = `${paths.meta}.${suffix}.tmp`;
        await Promise.all([
            writeFile(bundleTemp, canonical, { encoding: 'utf8', mode: 0o600 }),
            writeFile(metaTemp, `${JSON.stringify(meta)}\n`, { encoding: 'utf8', mode: 0o600 })
        ]);
        if (previous) {
            await mkdir(paths.backups, { recursive: true });
            await Promise.all([
                copyFile(paths.bundle, join(paths.backups, `revision-${String(previous.revision).padStart(8, '0')}.json`)),
                copyFile(paths.meta, join(paths.backups, `revision-${String(previous.revision).padStart(8, '0')}.meta.json`))
            ]);
        }
        await rename(bundleTemp, paths.bundle);
        await rename(metaTemp, paths.meta);
        await this.pruneBackups(paths.backups);
        return { steamId, revision, sha256: digest, savedAtMs, idempotencyKey, bundle };
    }
    async pruneBackups(root) {
        let entries;
        try {
            entries = await readdir(root);
        }
        catch {
            return;
        }
        const revisions = [...new Set(entries
                .map((name) => /^revision-(\d{8})\./.exec(name)?.[1])
                .filter((value) => Boolean(value)))].sort().reverse();
        for (const revision of revisions.slice(this.keepBackups)) {
            await Promise.all([
                rm(join(root, `revision-${revision}.json`), { force: true }),
                rm(join(root, `revision-${revision}.meta.json`), { force: true })
            ]);
        }
    }
}
//# sourceMappingURL=store.js.map