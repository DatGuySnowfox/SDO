const setBlocking = (stream) => {
    const handle = stream._handle;
    handle?.setBlocking?.(true);
};
export function ensureUnbufferedOutput() {
    if (process.stdout.isTTY && process.stderr.isTTY)
        return;
    try {
        setBlocking(process.stdout);
        setBlocking(process.stderr);
    }
    catch {
        // Best effort; redirected logs may still flush late on some hosts.
    }
}
export function log(level, event, context = {}) {
    const safe = Object.fromEntries(Object.entries(context)
        .filter(([key]) => !/(secret|token|password|authorization|private.?key)/i.test(key))
        .map(([key, value]) => [key, typeof value === 'bigint' ? value.toString() : value]));
    process.stdout.write(`${JSON.stringify({
        timestamp: new Date().toISOString(),
        level,
        event,
        protocolVersion: 3,
        ...safe
    })}\n`);
}
//# sourceMappingURL=log.js.map