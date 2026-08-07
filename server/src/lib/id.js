'use strict';

const crypto = require('node:crypto');

// Derive a stable uint64 numeric ID from an arbitrary string key.
// Uses the first 8 bytes of SHA-256.  0 is remapped to 1 (reserved by protocol).
function stableNumericId(value) {
    const hash = crypto.createHash('sha256').update(value, 'utf8').digest();
    const id = hash.readBigUInt64BE(0);
    return id === 0n ? 1n : id;
}

module.exports = { stableNumericId };
