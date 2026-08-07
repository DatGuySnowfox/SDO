'use strict';

// UDP socket that faces the local game DLL.
//
// The game DLL sends frames to runtimePort (we bind here).
// We send frames to gameHost:bindPort (the DLL's listening port).
// Non-loopback datagrams are silently rejected for safety.

const dgram = require('node:dgram');
const cfg   = require('./config');

class UdpBridge {
    constructor(onFrame) {
        this._onFrame = onFrame; // (rawBuf: Buffer) => void
        this._socket  = null;
    }

    open() {
        const sock = dgram.createSocket('udp4');

        sock.on('error', (err) => {
            console.error('[udp] socket error:', err.message);
        });

        sock.on('message', (msg, rinfo) => {
            // Accept only loopback – the game DLL is always local.
            if (rinfo.address !== '127.0.0.1') return;
            this._onFrame(msg);
        });

        sock.bind(cfg.runtimePort, '127.0.0.1', () => {
            console.log(`[udp] listening on 127.0.0.1:${cfg.runtimePort}`);
        });

        this._socket = sock;
    }

    send(buf) {
        if (this._socket)
            this._socket.send(buf, cfg.bindPort, cfg.gameHost);
    }

    close() {
        if (this._socket) {
            this._socket.close();
            this._socket = null;
        }
    }
}

module.exports = { UdpBridge };
