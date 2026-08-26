import { randomUUID } from 'node:crypto';
import { mkdir, readFile, readdir, rename, copyFile, stat } from 'node:fs/promises';
import { dirname, join, resolve } from 'node:path';
import { PROFILE_BUNDLE_VERSION, profileContentSha256, sha256, validProfilePath, validateProfileBundle } from "../profile-service/bundle.js";
import { log } from "../shared/log.js";
export class ProfileSync {
    config;
    sessionToken = '';
    steamId = '';
    revision = 0;
    lastSignature = '';
    lastSavedAtMs = 0;
    lastError = '';
    savesCompleted = 0;
    restoresCompleted = 0;
    timer;
    operation;
    constructor(config) {
        this.config = config;
    }
    status() {
        return {
            enabled: true,
            ready: Boolean(this.sessionToken && this.steamId),
            steamId: this.steamId,
            revision: this.revision,
            lastSavedAtMs: this.lastSavedAtMs,
            savesCompleted: this.savesCompleted,
            restoresCompleted: this.restoresCompleted,
            lastError: this.lastError
        };
    }
    async start() {
        const response = await fetch(`${this.config.baseUrl.replace(/\/+$/, '')}/v1/sessions`, {
            method: 'POST',
            headers: { authorization: `Bearer ${this.config.joinTicket}` },
            signal: AbortSignal.timeout(10_000)
        });
        if (!response.ok)
            throw new Error(`profile_session_failed_${response.status}`);
        const session = await response.json();
        this.steamId = session.steamId;
        this.sessionToken = session.token;
        await this.restoreOrSeed();
        this.timer = setInterval(() => void this.checkpoint('periodic'), Math.max(5_000, this.config.checkpointMs ?? 15_000));
        this.timer.unref();
    }
    async stop() {
        if (this.timer)
            clearInterval(this.timer);
        await this.checkpoint('final', true);
    }
    async checkpoint(reason = 'manual', force = false) {
        if (this.operation) {
            await this.operation;
            if (!force)
                return;
        }
        this.operation = this.checkpointInner(reason, force)
            .finally(() => {
            this.operation = undefined;
        });
        return this.operation;
    }
    async restoreOrSeed() {
        const response = await fetch(this.profileUrl(), {
            headers: this.headers(),
            signal: AbortSignal.timeout(15_000)
        });
        if (response.status === 404) {
            await this.checkpoint('initial-seed', true);
            return;
        }
        if (!response.ok)
            throw new Error(`profile_restore_failed_${response.status}`);
        const value = await response.json();
        const bundle = validateProfileBundle(value.bundle, this.steamId);
        await this.restoreBundle(bundle);
        this.revision = value.revision;
        this.lastSignature = value.sha256;
        this.restoresCompleted += 1;
        this.lastError = '';
        log('info', 'profile_restored', {
            playerId: this.steamId,
            saveRevision: this.revision
        });
    }
    async checkpointInner(reason, force) {
        try {
            const bundle = await this.captureBundle();
            const signature = profileContentSha256(bundle);
            if (!force && signature === this.lastSignature)
                return;
            const response = await fetch(this.profileUrl(), {
                method: 'PUT',
                headers: {
                    ...this.headers(),
                    'content-type': 'application/json',
                    'x-profile-base-revision': String(this.revision),
                    'x-idempotency-key': `${reason}:${randomUUID()}`
                },
                body: JSON.stringify(bundle),
                signal: AbortSignal.timeout(20_000)
            });
            if (response.status === 409) {
                const conflict = await response.json();
                throw new Error(`profile_revision_conflict_${conflict.currentRevision ?? 'unknown'}`);
            }
            if (!response.ok)
                throw new Error(`profile_save_failed_${response.status}`);
            const value = await response.json();
            this.revision = value.revision;
            this.lastSignature = value.sha256;
            this.lastSavedAtMs = value.savedAtMs;
            this.savesCompleted += 1;
            this.lastError = '';
            log('info', 'profile_saved', {
                playerId: this.steamId,
                saveRevision: this.revision,
                checkpointReason: reason
            });
        }
        catch (error) {
            this.lastError = error instanceof Error ? error.message : 'profile_save_failed';
            log('error', 'profile_save_error', {
                playerId: this.steamId,
                saveRevision: this.revision,
                checkpointReason: reason,
                error: this.lastError
            });
            if (force)
                throw error;
        }
    }
    async captureBundle() {
        for (let attempt = 0; attempt < 3; attempt += 1) {
            const first = await this.captureBundleOnce();
            await new Promise((resolveDelay) => setTimeout(resolveDelay, 200));
            const second = await this.captureBundleOnce();
            if (profileContentSha256(first) === profileContentSha256(second))
                return second;
        }
        throw new Error('profile_files_did_not_stabilize');
    }
    async captureBundleOnce() {
        const files = [];
        for (const relativePath of await this.discoverProfilePaths()) {
            const path = this.safePath(relativePath);
            let data;
            for (let attempt = 0; attempt < 3; attempt += 1) {
                try {
                    data = await readFile(path);
                    break;
                }
                catch {
                    if (attempt < 2)
                        await new Promise((resolveDelay) => setTimeout(resolveDelay, 100));
                }
            }
            if (!data || data.length === 0) {
                throw new Error(`profile_file_missing_${relativePath.replaceAll('/', '_')}`);
            }
            files.push({
                path: relativePath,
                bytes: data.length,
                sha256: sha256(data),
                data: data.toString('base64')
            });
        }
        return validateProfileBundle({
            version: PROFILE_BUNDLE_VERSION,
            steamId: this.steamId,
            createdAtMs: Date.now(),
            files
        }, this.steamId);
    }
    async discoverProfilePaths() {
        const paths = [];
        const rootEntries = await readdir(this.config.saveRoot, { withFileTypes: true });
        for (const entry of rootEntries) {
            if (entry.isFile() && validProfilePath(entry.name)) {
                paths.push(entry.name);
                continue;
            }
            if (!entry.isDirectory() || !/^[A-Za-z0-9_-]{1,64}$/.test(entry.name))
                continue;
            const directoryEntries = await readdir(this.safePath(entry.name), { withFileTypes: true });
            for (const child of directoryEntries) {
                const relativePath = `${entry.name}/${child.name}`;
                if (child.isFile() && validProfilePath(relativePath))
                    paths.push(relativePath);
            }
        }
        return paths.sort((left, right) => left.localeCompare(right));
    }
    async restoreBundle(bundle) {
        const backupRoot = join(dirname(this.config.saveRoot), 'SDO-ProfileBackups', `${this.steamId}-${Date.now()}`);
        await mkdir(backupRoot, { recursive: true });
        for (const file of bundle.files) {
            const destination = this.safePath(file.path);
            const data = Buffer.from(file.data, 'base64');
            await mkdir(dirname(destination), { recursive: true });
            try {
                if ((await stat(destination)).isFile()) {
                    const backup = join(backupRoot, file.path);
                    await mkdir(dirname(backup), { recursive: true });
                    await copyFile(destination, backup);
                }
            }
            catch {
                // A missing local file is restored from the server without a backup.
            }
            const temporary = `${destination}.${process.pid}.restore.tmp`;
            await import('node:fs/promises').then(({ writeFile }) => writeFile(temporary, data, { mode: 0o600 }));
            await rename(temporary, destination);
        }
    }
    safePath(relativePath) {
        const root = resolve(this.config.saveRoot);
        const destination = resolve(root, ...relativePath.split('/'));
        if (!destination.startsWith(`${root}\\`) && destination !== root) {
            throw new Error('profile_path_escape');
        }
        return destination;
    }
    profileUrl() {
        return `${this.config.baseUrl.replace(/\/+$/, '')}/v1/profiles/${encodeURIComponent(this.steamId)}`;
    }
    headers() {
        return { authorization: `Bearer ${this.sessionToken}` };
    }
}
//# sourceMappingURL=profile-sync.js.map