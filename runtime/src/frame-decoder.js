'use strict';

// Accumulates raw TCP chunks and slices out complete protocol frames.
// TCP streams may split or coalesce datagrams, so we reassemble here.

const { FRAME_MAGIC, HEADER_SIZE, MAX_PAYLOAD, decodeFrame } = require('./protocol');

const MAX_BUFFER = (HEADER_SIZE + MAX_PAYLOAD) * 4; // 263 KB safety cap

class FrameDecoder {
    constructor() {
        this._buf = Buffer.alloc(0);
    }

    // Append a TCP data chunk and return an array of complete frames parsed from it.
    push(chunk) {
        this._buf = Buffer.concat([this._buf, chunk]);

        if (this._buf.length > MAX_BUFFER)
            throw new Error('receive_buffer_exceeded');

        const frames = [];
        while (this._buf.length >= HEADER_SIZE) {
            const magic = this._buf.readUInt32BE(0);
            if (magic !== FRAME_MAGIC)
                throw new Error(`bad_magic: 0x${magic.toString(16)}`);

            const payloadLength = this._buf.readUInt32BE(12);
            if (payloadLength > MAX_PAYLOAD)
                throw new Error(`payload_too_large: ${payloadLength}`);

            const total = HEADER_SIZE + payloadLength;
            if (this._buf.length < total) break;

            frames.push(decodeFrame(this._buf.subarray(0, total)));
            this._buf = this._buf.subarray(total);
        }
        return frames;
    }

    reset() {
        this._buf = Buffer.alloc(0);
    }
}

module.exports = { FrameDecoder };
