import { createSocket } from 'node:dgram';
import { decodeFrame, encodeFrame } from "../shared-protocol/index.js";
import { log } from "../shared/log.js";
function isLoopback(address) {
    return address === '127.0.0.1' || address === '::1' || address === '::ffff:127.0.0.1';
}
export class UdpGameBridge {
    config;
    socket;
    onLocalFrame;
    ready = false;
    listenPort = 0;
    localFrames = 0;
    remoteFrames = 0;
    constructor(config) {
        this.config = config;
    }
    async start(onLocalFrame) {
        this.onLocalFrame = onLocalFrame;
        this.socket = createSocket('udp4');
        this.socket.on('message', (message, remote) => this.receive(message, remote));
        this.socket.on('error', (error) => log('error', 'game_bridge_socket_error', {
            error: error.message
        }));
        await new Promise((resolve, reject) => {
            this.socket.once('error', reject);
            this.socket.bind(this.config.listenPort, this.config.host ?? '127.0.0.1', resolve);
        });
        this.listenPort = this.socket.address().port;
        this.ready = true;
        log('info', 'game_bridge_started', {
            mode: 'protocol-v3-udp-loopback',
            listenPort: this.listenPort,
            gamePort: this.config.gamePort
        });
    }
    send(frame) {
        if (!this.socket || !this.ready)
            return;
        const bytes = encodeFrame(frame);
        this.socket.send(bytes, this.config.gamePort, this.config.host ?? '127.0.0.1');
        this.remoteFrames += 1;
    }
    status() {
        return {
            mode: 'protocol-v3-udp-loopback',
            ready: this.ready,
            localFrames: this.localFrames,
            remoteFrames: this.remoteFrames,
            listenPort: this.listenPort,
            gamePort: this.config.gamePort
        };
    }
    async stop() {
        if (!this.ready)
            return;
        this.ready = false;
        if (this.socket) {
            await new Promise((resolve) => this.socket.close(() => resolve()));
            this.socket = undefined;
        }
    }
    receive(message, remote) {
        if (!isLoopback(remote.address)) {
            log('warning', 'game_bridge_non_loopback_rejected', { address: remote.address });
            return;
        }
        try {
            const frame = decodeFrame(message);
            this.localFrames += 1;
            this.onLocalFrame?.(frame);
        }
        catch (error) {
            log('warning', 'game_bridge_frame_rejected', {
                error: error instanceof Error ? error.message : 'unknown'
            });
        }
    }
}
//# sourceMappingURL=udp-game-bridge.js.map