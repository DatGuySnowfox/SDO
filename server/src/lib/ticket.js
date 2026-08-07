'use strict';

const crypto = require('node:crypto');

// Ticket format: "<base64url(JSON)>.<base64url(HMAC-SHA256)>"
// The JSON body contains: ticketId, playerId, displayName, worldId,
//                         expiresAtMs, protocolVersion.

function signTicket(body, secret) {
    const encoded = Buffer.from(JSON.stringify(body), 'utf8').toString('base64url');
    const sig = crypto.createHmac('sha256', secret).update(encoded).digest().toString('base64url');
    return `${encoded}.${sig}`;
}

// Returns the parsed body, or throws a descriptive Error.
function verifyTicket(ticket, secret, expectedWorldId) {
    const dot = ticket.indexOf('.');
    if (dot < 0) throw new Error('invalid_ticket');

    const encoded = ticket.slice(0, dot);
    const sig     = ticket.slice(dot + 1);

    const expected = crypto.createHmac('sha256', secret).update(encoded).digest().toString('base64url');
    const sigBuf = Buffer.from(sig,      'base64url');
    const expBuf = Buffer.from(expected, 'base64url');
    if (sigBuf.length !== expBuf.length || !crypto.timingSafeEqual(sigBuf, expBuf))
        throw new Error('invalid_ticket_signature');

    let body;
    try { body = JSON.parse(Buffer.from(encoded, 'base64url').toString('utf8')); }
    catch { throw new Error('invalid_ticket'); }

    if (body.protocolVersion !== 3)   throw new Error('invalid_ticket');
    if (body.expiresAtMs < Date.now()) throw new Error('expired_ticket');
    if (expectedWorldId && body.worldId !== expectedWorldId) throw new Error('wrong_world');

    return body;
}

module.exports = { signTicket, verifyTicket };
