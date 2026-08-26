import { PROTOCOL_VERSION } from "../shared-protocol/index.js";
export async function requestJoinTicket(input) {
    const response = await fetch(input.ticketUrl, {
        method: 'POST',
        headers: {
            'content-type': 'application/json',
            authorization: `Bearer ${input.identityToken}`
        },
        body: JSON.stringify({
            playerId: input.playerId,
            displayName: input.displayName
        }),
        signal: AbortSignal.timeout(5_000)
    });
    if (!response.ok) {
        throw new Error(`Ticket request failed with HTTP ${response.status}`);
    }
    const value = await response.json();
    if (value.protocolVersion !== PROTOCOL_VERSION ||
        typeof value.ticket !== 'string' ||
        typeof value.gatewayHost !== 'string' ||
        typeof value.gatewayPort !== 'number' ||
        typeof value.worldId !== 'string' ||
        typeof value.expiresAtMs !== 'number') {
        throw new Error('Ticket response was invalid or used another protocol version');
    }
    if (value.expiresAtMs <= Date.now()) {
        throw new Error('Gateway returned an expired ticket');
    }
    return value;
}
//# sourceMappingURL=ticket.js.map