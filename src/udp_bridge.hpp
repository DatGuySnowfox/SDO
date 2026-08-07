#pragma once
#include "protocol.hpp"
#include <cstdint>
#include <vector>

// WinSock forward
#ifdef _WIN32
#  include <winsock2.h>
using sdb_socket_t = SOCKET;
static constexpr sdb_socket_t SDB_INVALID_SOCKET = INVALID_SOCKET;
#else
using sdb_socket_t = int;
static constexpr sdb_socket_t SDB_INVALID_SOCKET = -1;
#endif

namespace sdb {

// UDP loopback bridge between the UE5 game and the Node.js bridge runtime.
//
//   bind_port   – port the DLL listens on (game side)   default 42100
//   runtime_port – port the bridge runtime listens on   default 42101
//
// Both sides must agree; configure via env vars SDB_BIND_PORT / SDB_RUNTIME_PORT.
class UdpBridge {
public:
    UdpBridge() = default;
    ~UdpBridge() { shutdown(); }

    // Non-copyable
    UdpBridge(const UdpBridge&)            = delete;
    UdpBridge& operator=(const UdpBridge&) = delete;

    bool open(uint16_t bind_port, uint16_t runtime_port);

    // Send a pre-encoded frame buffer (e.g. from encode_frame)
    bool send_raw(const uint8_t* data, int len);

    // Non-blocking drain of the receive buffer.
    // Returns all complete, validated frames.
    std::vector<Frame> recv_all();

    bool is_open() const { return sock_ != SDB_INVALID_SOCKET; }
    void shutdown();

private:
    sdb_socket_t sock_         = SDB_INVALID_SOCKET;
    uint16_t     runtime_port_ = 0;
    uint8_t      recv_buf_[FRAME_HEADER_SIZE + FRAME_MAX_PAYLOAD]{};
};

} // namespace sdb
