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