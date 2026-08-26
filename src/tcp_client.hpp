#pragma once
#include "protocol.hpp"
#include <atomic>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
#  include <ws2tcpip.h>
using sdb_socket_t = SOCKET;
static constexpr sdb_socket_t SDB_INVALID_SOCKET = INVALID_SOCKET;
#else
#  include <netdb.h>
#  include <sys/socket.h>
#  include <sys/select.h>
using sdb_socket_t = int;
static constexpr sdb_socket_t SDB_INVALID_SOCKET = -1;
#endif

namespace sdb {

// TCP client that connects directly to the gateway server.
//
// Background thread owns all socket I/O.  Game thread (on_engine_tick)
// calls send_bytes() / recv_all() which are thread-safe queue ops.
//
// State machine (TCP thread only):
//   Disconnected → Auth → Joining → Active
//   Any error → Disconnected → reconnect with exponential backoff.
class TcpClient {
public:
    TcpClient() = default;
    ~TcpClient() { shutdown(); }

    TcpClient(const TcpClient&)            = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    // Spawn the background I/O thread.  host is hostname or dotted-IP.
    void open(std::string host, uint16_t port, std::string ticket);

    // Enqueue a pre-encoded frame buffer (game thread, non-blocking).
    void send_bytes(const uint8_t* data, int len);

    // Drain all received frames (game thread, replaces UdpBridge::recv_all).
    std::vector<Frame> recv_all();

    // True once JoinAccepted has been received and the session is seeded.
    bool is_joined() const;

    // True while the background thread is alive.
    bool is_open() const { return alive_.load(std::memory_order_relaxed); }

    void shutdown();

private:
    enum class ConnState : uint8_t { Disconnected, Auth, Joining, Active };

    std::string  host_;
    uint16_t     port_   = 0;
    std::string  ticket_;

    std::atomic<bool>      alive_  { false };
    std::atomic<bool>      stopped_{ false };
    std::atomic<ConnState> state_  { ConnState::Disconnected };
    std::thread            thread_;

    // Inbound queue: TCP thread writes, game thread reads
    std::mutex         inMtx_;
    std::deque<Frame>  inQueue_;

    // Outbound queue: game thread writes, TCP thread drains
    std::mutex                       outMtx_;
    std::deque<std::vector<uint8_t>> outQueue_;

    // ── All fields below are ONLY touched by the TCP thread ────────────────
    // ...with one deliberate exception: sock_ is also written by shutdown()
    // (tcp_client.cpp), called from whatever thread owns this TcpClient at
    // mod-unload (game thread, not the TCP thread) via close_socket().
    // That's intentional, not an oversight — closesocket()/close() on a
    // socket another thread is blocked on inside select()/recv() is a
    // standard, well-defined way to unblock it (both Winsock and POSIX
    // guarantee this), and it's the only way shutdown() can make the TCP
    // thread's select() loop (run_connected(), below) exit promptly instead
    // of waiting out up to one full SELECT_TIMEOUT_US. Audited 2026-08-16:
    // the only cross-thread call is this one, exactly once, at teardown —
    // never during normal operation — so there's no concurrent-mutation
    // window while the TCP thread is actively using sock_ for anything
    // other than the same select()/recv() call this is meant to interrupt.
    sdb_socket_t         sock_         = SDB_INVALID_SOCKET;
    std::vector<uint8_t> recvBuf_;
    uint64_t             lastHbUs_     = 0;
    uint32_t             seqCounter_   = 0;
    uint32_t             tickCounter_  = 0;
    bool                 killConn_     = false;  // signal from dispatch to run_connected

    // Session fields latched after auth / join
    uint64_t connId_   = 0;
    Uuid16   sessionId_{};
    Uuid16   worldId_  {};
    uint64_t playerId_ = 0;
    uint64_t entityId_ = 0;

    void thread_func();
    bool try_connect();
    void run_connected();
    bool recv_chunk();
    void dispatch_tcp_frame(Frame& f);
    void send_authenticate();
    void send_join_request();
    void send_heartbeat();
    void seed_game_session();
    int  write_all(const uint8_t* data, int len);
    void drain_outbound();
    void close_socket();
    void reset_state();
};

} // namespace sdb
