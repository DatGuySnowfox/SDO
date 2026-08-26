import { randomUUID } from 'node:crypto';
import { mkdir, readFile, readdir, rename, copyFile, stat } from 'node:fs/promises';
import { dirname, join, resolve } from 'node:path';
import { PROFILE_BUNDLE_VERSION, profileContentSha256, sha256, validProfilePath, validateProfileBundle } from "../profile-service/bundle.js";
import { log } from "../shared/log.js";
export class ProfileSync {
    config;
    baseUrls;
    activeBaseUrl = '';
    sessionToken = '';
    steamId = '';
    revision = 0;
    lastSignature = '';
    lastSavedAtMs = 0;
    lastError = '';
    savesCompleted = 0;
    restoresCompleted = 0;
    recoveryMode = false;
    timer;
    operation;
    constructor(config) {
        this.config = config;
        const urls = (config.baseUrls?.length ? config.baseUrls : [config.baseUrl])
            .map((url) => url.replace(/\/+$/, ''))
            .filter(Boolean);
        this.baseUrls = [...new Set(urls)];
        this.activeBaseUrl = this.baseUrls[0] ?? '';
    }
    status() {
        return {
            enabled: true,
            serverCanonical: this.config.serverCanonical === true,
            ready: Boolean(this.sessionToken && this.steamId),
            steamId: this.steamId,
            revision: this.revision,
            lastSavedAtMs: this.lastSavedAtMs,
            savesCompleted: this.savesCompleted,
            restoresCompleted: this.restoresCompleted,
            recoveryMode: this.recoveryMode,
            lastError: this.lastError
        };
    }
    async start() {
        const response = await this.request('/v1/sessions', {
            method: 'POST',
            headers: { authorization: `Bearer ${this.config.joinTicket}` },
            signal: AbortSignal.timeout(10_000)
        }, 3);
        if (!response.ok)
            throw new Error(`profile_session_failed_${response.status}`);
        const session = await response.json();
        this.steamId = session.steamId;
        this.sessionToken = session.token;
        await this.restoreOrSeed();
        this.timer = setInterval(() => void this.checkpoint('periodic'), Math.max(5_000, this.config.checkpointMs ?? 5_000));
        this.timer.unref();
    }
    async stop() {
        if (this.timer)
            clearInterval(this.timer);
        if (!this.recoveryMode)
            await this.checkpoint('final', true);
    }
    async checkpoint(reason = 'manual', force = false) {
        if (this.recoveryMode)
            return;
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
    async recoverPrevious(reason = 'bulkdata-crash') {
        if (this.operation)
            await this.operation;
        const response = await this.request(`${this.profilePath()}/recover`, {
            method: 'POST',
            headers: {
                ...this.headers(),
                'content-type': 'application/json'
            },
            body: JSON.stringify({
                expectedRevision: this.revision,
                reason: reason.slice(0, 64)
            }),
            signal: AbortSignal.timeout(15_000)
        });
        const value = await response.json();
        if (!response.ok)
            throw new Error(value.error ?? `profile_recovery_failed_${response.status}`);
        if (!Number.isSafeInteger(value.revision) ||
            !Number.isSafeInteger(value.recoveredFromRevision) ||
            typeof value.sha256 !== 'string') {
            throw new Error('profile_recovery_response_invalid');
        }
        this.revision = value.revision;
        this.lastSignature = value.sha256;
        this.recoveryMode = true;
        this.lastError = '';
        log('warning', 'profile_recovered_after_bulkdata_crash', {
            playerId: this.steamId,
            saveRevision: this.revision,
            recoveredFromRevision: value.recoveredFromRevision
        });
        return {
            revision: this.revision,
            recoveredFromRevision: value.recoveredFromRevision
        };
    }
    async restoreOrSeed() {
        const response = await this.request(this.profilePath(), {
            headers: this.headers(),
            signal: AbortSignal.timeout(15_000)
        });
        if (response.status === 404) {
            if (await this.hasDiscoverableProfileFiles()) {
                try {
                    await this.checkpoint('initial-seed', true);
                    if (this.revision === 0 && this.isDeferrableSeedError(this.lastError)) {
                        this.lastError = this.deferSeedError(this.lastError);
                        log('warning', 'profile_seed_deferred', {
                            playerId: this.steamId,
                            saveRevision: this.revision,
                            reason: this.lastError
                        });
                    }
                }
                catch (error) {
                    this.lastError = this.deferSeedError(error);
                    log('warning', 'profile_seed_deferred', {
                        playerId: this.steamId,
                        saveRevision: this.revision,
                        reason: this.lastError
                    });
                }
            }
            else {
                this.lastError = 'profile_seed_deferred_until_save_exists';
                log('warning', 'profile_seed_deferred', {
                    playerId: this.steamId,
                    saveRevision: this.revision,
                    reason: this.lastError
                });
            }
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
            const response = await this.request(this.profilePath(), {
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
            if (force &&
                this.revision === 0 &&
                this.isDeferrableSeedError(this.lastError)) {
                log('warning', 'profile_final_seed_skipped', {
                    playerId: this.steamId,
                    saveRevision: this.revision,
                    reason: this.lastError
                });
                return;
            }
            if (force)
                throw error;
        }
    }
    deferSeedError(error) {
        const message = typeof error === 'string'
            ? error
            : error instanceof Error
                ? error.message
                : 'profile_seed_failed';
        return this.isDeferrableSeedError(message)
            ? `profile_seed_deferred_${message}`
            : `profile_seed_deferred_${message.slice(0, 96)}`;
    }
    isDeferrableSeedError(message) {
        return message === 'profile_no_save_files_found' ||
            message === 'profile_files_did_not_stabilize' ||
            message === 'profile_bundle_invalid' ||
            message === 'profile_bundle_too_large' ||
            message.startsWith('profile_file_') ||
            message.startsWith('profile_save_failed_');
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
        const profilePaths = await this.discoverProfilePaths();
        if (profilePaths.length === 0)
            throw new Error('profile_no_save_files_found');
        for (const relativePath of profilePaths) {
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
        let rootEntries;
        try {
            rootEntries = await readdir(this.config.saveRoot, { withFileTypes: true });
        }
        catch (error) {
            if (error &&
                typeof error === 'object' &&
                'code' in error &&
                error.code === 'ENOENT') {
                return [];
            }
            throw error;
        }
        for (const entry of rootEntries) {
            if (entry.isFile() &&
                validProfilePath(entry.name) &&
                (!this.config.serverCanonical || this.isCanonicalServerPath(entry.name))) {
                paths.push(entry.name);
                continue;
            }
            if (!entry.isDirectory() ||
                !/^[A-Za-z0-9_-]{1,64}$/.test(entry.name) ||
                (this.config.serverCanonical && entry.name !== 'SaveSlot'))
                continue;
            const directoryEntries = await readdir(this.safePath(entry.name), { withFileTypes: true });
            for (const child of directoryEntries) {
                const relativePath = `${entry.name}/${child.name}`;
                if (child.isFile() &&
                    validProfilePath(relativePath) &&
                    (!this.config.serverCanonical || this.isCanonicalServerPath(relativePath)))
                    paths.push(relativePath);
            }
        }
        return paths.sort((left, right) => left.localeCompare(right));
    }
    async hasDiscoverableProfileFiles() {
        return (await this.discoverProfilePaths()).length > 0;
    }
    async restoreBundle(bundle) {
        const backupRoot = join(dirname(this.config.saveRoot), 'SDO-ProfileBackups', `${this.steamId}-${Date.now()}`);
        await mkdir(backupRoot, { recursive: true });
        const files = this.config.serverCanonical
            ? bundle.files.filter((file) => this.isCanonicalServerPath(file.path))
            : bundle.files;
        if (files.length === 0)
            throw new Error('profile_canonical_slot_missing');
        for (const file of files) {
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
    isCanonicalServerPath(path) {
        return path === 'SurrounDead.sav' ||
            path === 'SaveSlot_Quests.sav' ||
            path === 'SaveSlot_SkillTree.sav' ||
            /^SaveSlot\/(?:Difficulties|General|Level|Player|PlayerInfo|Slot)\.sav$/.test(path) ||
            path === 'SaveSlot/Thumb.jpg';
    }
    safePath(relativePath) {
        const root = resolve(this.config.saveRoot);
        const destination = resolve(root, ...relativePath.split('/'));
        if (!destination.startsWith(`${root}\\`) && destination !== root) {
            throw new Error('profile_path_escape');
        }
        return destination;
    }
    profilePath() {
        return `/v1/profiles/${encodeURIComponent(this.steamId)}`;
    }
    async request(path, init, attempts = 1) {
        const ordered = [
            this.activeBaseUrl,
            ...this.baseUrls.filter((url) => url !== this.activeBaseUrl)
        ].filter(Boolean);
        let lastError;
        for (let attempt = 0; attempt < Math.max(1, attempts); attempt += 1) {
            for (const base of ordered) {
                try {
                    const response = await fetch(`${base}${path}`, init);
                    this.activeBaseUrl = base;
                    return response;
                }
                catch (error) {
                    lastError = error;
                    log('warning', 'profile_request_failed', {
                        playerId: this.steamId || undefined,
                        profileBaseUrl: base,
                        attempt: attempt + 1,
                        error: error instanceof Error ? error.message : 'profile_request_failed'
                    });
                }
            }
            if (attempt + 1 < attempts) {
                await new Promise((resolveDelay) => setTimeout(resolveDelay, 250 * (attempt + 1)));
            }
        }
        throw lastError instanceof Error ? lastError : new Error('profile_request_failed');
    }
    headers() {
        return { authorization: `Bearer ${this.sessionToken}` };
    }
}
//# sourceMappingURL=profile-sync.js.map