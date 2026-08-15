#include "tcp_client.hpp"

#include <RC/DynamicOutput/Output.hpp>

#include <algorithm>
#include <cstring>
#include <cstdio>

#ifdef _WIN32
// winsock2.h + ws2tcpip.h (via tcp_client.hpp) provide TCP_NODELAY
#else
#  include <fcntl.h>
#  include <netinet/tcp.h>
#  include <unistd.h>
#endif

using namespace RC;

namespace sdb {

static constexpr uint64_t HB_INTERVAL_US  = 1'000'000ULL;   // 1 s
static constexpr uint64_t SELECT_TIMEOUT_US = 10'000ULL;    // 10 ms

// ── WSA lifetime ─────────────────────────────────────────────────────────────

#ifdef _WIN32
namespace {
struct WsaInit {
    WsaInit()  { WSADATA d{}; WSAStartup(MAKEWORD(2, 2), &d); }
    ~WsaInit() { WSACleanup(); }
};
static WsaInit g_wsa;
}
#endif

// ── Helpers ───────────────────────────────────────────────────────────────────

static void sleep_ms(uint32_t ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(static_cast<useconds_t>(ms) * 1000u);
#endif
}

// Encode a Frame into a heap buffer using the module-level protocol encoder.
// seq and tick are the TcpClient's own per-connection counters.
static std::vector<uint8_t> frame_to_buf(Frame& f, uint32_t& seq, uint32_t& tick)
{
    std::vector<uint8_t> buf(FRAME_HEADER_SIZE + FRAME_MAX_PAYLOAD);
    int n = encode_frame(buf.data(), static_cast<int>(buf.size()), f, seq, tick);
    if (n <= 0) return {};
    buf.resize(static_cast<size_t>(n));
    return buf;
}

// ── Public API ────────────────────────────────────────────────────────────────

void TcpClient::open(std::string host, uint16_t port, std::string ticket)
{
    host_   = std::move(host);
    port_   = port;
    ticket_ = std::move(ticket);
    stopped_.store(false, std::memory_order_relaxed);
    alive_.store(true,    std::memory_order_relaxed);
    thread_ = std::thread([this] { thread_func(); });
}

void TcpClient::send_bytes(const uint8_t* data, int len)
{
    if (len <= 0) return;
    std::lock_guard<std::mutex> lk(outMtx_);
    outQueue_.emplace_back(data, data + len);
}

std::vector<Frame> TcpClient::recv_all()
{
    std::lock_guard<std::mutex> lk(inMtx_);
    std::vector<Frame> out(std::make_move_iterator(inQueue_.begin()),
                           std::make_move_iterator(inQueue_.end()));
    inQueue_.clear();
    return out;
}

bool TcpClient::is_joined() const
{
    return state_.load(std::memory_order_relaxed) == ConnState::Active;
}

void TcpClient::shutdown()
{
    stopped_.store(true, std::memory_order_relaxed);
    close_socket(); // unblocks select()
    if (thread_.joinable()) thread_.join();
    alive_.store(false, std::memory_order_relaxed);
}

// ── Background thread ─────────────────────────────────────────────────────────

void TcpClient::thread_func()
{
    uint32_t reconnectMs = 250;

    // Backoff applies to EVERY disconnect reason alike — a raw connect()
    // failure, an auth/join rejection, or a session that ran fine and then
    // dropped. Earlier this only guarded the connect()-failure branch and
    // reset to the 250ms floor on every successful TCP-level connect
    // regardless of what happened next; since the gateway can now be
    // configured to allow ticket replay (cfg.ticketReplayProtection=false,
    // a dev/LAN-only escape hatch — see gateway.js — until the launcher
    // owns fetching a fresh ticket per reconnect), a rejected/expired
    // ticket against a *protected* server would otherwise tight-loop
    // reconnecting with zero delay between attempts. 2026-08-13: exactly
    // that spun for long enough to leak ~24,000 TIME_WAIT sockets on port
    // 42200 and exhaust the machine's whole ephemeral port range. Only a
    // real, fully-joined (Active) session resets the backoff floor now.
    while (!stopped_.load(std::memory_order_relaxed)) {
        state_.store(ConnState::Disconnected, std::memory_order_relaxed);

        if (!try_connect()) {
            Output::send<LogLevel::Warning>(
                STR("[tcp] connect failed, retry in {:d}ms\n"), reconnectMs);
        } else {
            reset_state();
            const uint64_t connectedAtUs = now_micros();
            Output::send<LogLevel::Normal>(STR("[tcp] connected to gateway\n"));

            send_authenticate();
            state_.store(ConnState::Auth, std::memory_order_relaxed);

            run_connected();

            const bool wasActive = state_.load(std::memory_order_relaxed) == ConnState::Active;
            const uint64_t aliveMs = (now_micros() - connectedAtUs) / 1000;
            close_socket();

            if (stopped_.load(std::memory_order_relaxed)) break;

            if (wasActive) {
                reconnectMs = 250;
                Output::send<LogLevel::Warning>(
                    STR("[tcp] disconnected after {:d}ms active, reconnecting in {:d}ms …\n"),
                    aliveMs, reconnectMs);
            } else {
                Output::send<LogLevel::Error>(
                    STR("[tcp] session rejected after {:d}ms (ticket spent/invalid), retry in {:d}ms\n"),
                    aliveMs, reconnectMs);
            }
        }

        for (uint32_t e = 0;
             e < reconnectMs && !stopped_.load(std::memory_order_relaxed);
             e += 10) {
            sleep_ms(10);
        }
        reconnectMs = std::min(reconnectMs * 2u, 5'000u);
    }

    alive_.store(false, std::memory_order_relaxed);
}

bool TcpClient::try_connect()
{
    char portStr[8];
    std::snprintf(portStr, sizeof(portStr), "%u", port_);

    struct addrinfo hints{};
    hints.ai_family   = AF_INET;   // gateway listens on IPv4 only
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* res = nullptr;
    if (getaddrinfo(host_.c_str(), portStr, &hints, &res) != 0) return false;

    sdb_socket_t s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == SDB_INVALID_SOCKET) { freeaddrinfo(res); return false; }

    if (connect(s, res->ai_addr, static_cast<int>(res->ai_addrlen)) != 0) {
        freeaddrinfo(res);
#ifdef _WIN32
        closesocket(s);
#else
        close(s);
#endif
        return false;
    }
    freeaddrinfo(res);

    // Reduce latency for small frames.
    int one = 1;
    setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
               reinterpret_cast<const char*>(&one), sizeof(one));

    // Non-blocking for select()-driven I/O loop.
#ifdef _WIN32
    u_long nb = 1;
    ioctlsocket(s, FIONBIO, &nb);
#else
    int fl = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, fl | O_NONBLOCK);
#endif

    sock_ = s;
    return true;
}

void TcpClient::reset_state()
{
    recvBuf_.clear();
    lastHbUs_    = 0;
    seqCounter_  = 0;
    tickCounter_ = 0;
    killConn_    = false;
    connId_      = 0;
    sessionId_   = {};
    worldId_     = {};
    playerId_    = 0;
    entityId_    = 0;
    // Discard any stale outbound frames from before reconnect.
    std::lock_guard<std::mutex> lk(outMtx_);
    outQueue_.clear();
}

void TcpClient::run_connected()
{
    while (!stopped_.load(std::memory_order_relaxed) && !killConn_) {
        if (sock_ == SDB_INVALID_SOCKET) break;

        fd_set rfds, wfds;
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(sock_, &rfds);

        {
            std::lock_guard<std::mutex> lk(outMtx_);
            if (!outQueue_.empty()) FD_SET(sock_, &wfds);
        }

        struct timeval tv{};
        tv.tv_usec = static_cast<long>(SELECT_TIMEOUT_US);
        int r = select(static_cast<int>(sock_) + 1, &rfds, &wfds, nullptr, &tv);

        if (r < 0) {
#ifdef _WIN32
            Output::send<LogLevel::Error>(STR("[tcp] select() failed, code={:d}\n"), WSAGetLastError());
#else
            Output::send<LogLevel::Error>(STR("[tcp] select() failed, errno={:d}\n"), errno);
#endif
            break;
        }

        if (r > 0) {
            if (FD_ISSET(sock_, &rfds) && !recv_chunk()) break;
            if (FD_ISSET(sock_, &wfds)) drain_outbound();
        }

        if (state_.load(std::memory_order_relaxed) == ConnState::Active) {
            const uint64_t now = now_micros();
            if (lastHbUs_ == 0 || now - lastHbUs_ >= HB_INTERVAL_US) {
                send_heartbeat();
                lastHbUs_ = now;
            }
        }
    }
}

// ── Receive ───────────────────────────────────────────────────────────────────

bool TcpClient::recv_chunk()
{
    uint8_t tmp[8192];
#ifdef _WIN32
    int n = recv(sock_, reinterpret_cast<char*>(tmp), sizeof(tmp), 0);
    if (n == SOCKET_ERROR) {
        const int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) return true;
        Output::send<LogLevel::Error>(STR("[tcp] recv() failed, code={:d}\n"), err);
        return false;
    }
#else
    ssize_t n = recv(sock_, tmp, sizeof(tmp), 0);
    if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return true;
        Output::send<LogLevel::Error>(STR("[tcp] recv() failed, errno={:d}\n"), errno);
        return false;
    }
#endif
    if (n == 0) {
        Output::send<LogLevel::Warning>(STR("[tcp] remote closed the connection\n"));
        return false;
    }

    recvBuf_.insert(recvBuf_.end(), tmp, tmp + n);

    // Reassemble and dispatch complete frames.
    while (recvBuf_.size() >= FRAME_HEADER_SIZE) {
        // payloadLength at offset 12, big-endian uint32
        const uint32_t payLen =
            (static_cast<uint32_t>(recvBuf_[12]) << 24) |
            (static_cast<uint32_t>(recvBuf_[13]) << 16) |
            (static_cast<uint32_t>(recvBuf_[14]) <<  8) |
            (static_cast<uint32_t>(recvBuf_[15]));

        if (payLen > FRAME_MAX_PAYLOAD) {
            Output::send<LogLevel::Error>(
                STR("[tcp] protocol error: payload length {:d} exceeds max {:d}\n"),
                payLen, static_cast<uint32_t>(FRAME_MAX_PAYLOAD));
            return false;
        }

        const size_t total = FRAME_HEADER_SIZE + payLen;
        if (recvBuf_.size() < total) break; // partial frame, wait

        auto frame = decode_frame(recvBuf_.data(), static_cast<int>(total));
        recvBuf_.erase(recvBuf_.begin(),
                       recvBuf_.begin() + static_cast<ptrdiff_t>(total));

        if (frame) {
            dispatch_tcp_frame(*frame);
            if (killConn_) return false;
        }
    }
    return true;
}

// ── State machine ─────────────────────────────────────────────────────────────

void TcpClient::dispatch_tcp_frame(Frame& f)
{
    const ConnState st = state_.load(std::memory_order_relaxed);

    // ── Auth phase ──────────────────────────────────────────────────────────
    if (st == ConnState::Auth) {
        if (f.type == MsgType::AuthenticationOk) {
            connId_   = f.connectionId;
            worldId_  = f.worldId;
            playerId_ = f.playerId;
            Output::send<LogLevel::Normal>(
                STR("[tcp] authenticated  playerId={:d}\n"), playerId_);
            // Gateway auto-joins after auth; no explicit JoinRequest needed.
            state_.store(ConnState::Joining, std::memory_order_relaxed);
        } else if (f.type == MsgType::AuthenticationFail) {
            Output::send<LogLevel::Error>(STR("[tcp] authentication rejected\n"));
            killConn_ = true;
        }
        return;
    }

    // ── Joining phase ───────────────────────────────────────────────────────
    if (st == ConnState::Joining) {
        if (f.type == MsgType::JoinAccepted) {
            sessionId_ = f.sessionId;
            entityId_  = f.entityId;
            Output::send<LogLevel::Normal>(
                STR("[tcp] joined  entityId={:d}\n"), entityId_);
            seed_game_session();
            state_.store(ConnState::Active, std::memory_order_relaxed);
        } else if (f.type == MsgType::JoinRejected) {
            Output::send<LogLevel::Error>(STR("[tcp] join rejected\n"));
            killConn_ = true;
        }
        return;
    }

    if (st != ConnState::Active) return;

    // ── Active phase ────────────────────────────────────────────────────────

    // Drop our own movement echo.
    if (f.type == MsgType::Movement && f.playerId == playerId_) return;

    std::lock_guard<std::mutex> lk(inMtx_);
    inQueue_.push_back(std::move(f));
}

// ── Senders (TCP thread) ──────────────────────────────────────────────────────

void TcpClient::send_authenticate()
{
    // decodeString on the server requires payload.length === length + 2 exactly.
    const size_t tickLen = std::min(ticket_.size(), size_t(4094));
    std::vector<uint8_t> payload(2 + tickLen);
    payload[0] = static_cast<uint8_t>(tickLen >> 8);
    payload[1] = static_cast<uint8_t>(tickLen & 0xFF);
    std::memcpy(payload.data() + 2, ticket_.data(), tickLen);

    Frame f;
    f.type    = MsgType::ClientAuthenticate;
    f.payload = std::move(payload);

    auto buf = frame_to_buf(f, seqCounter_, tickCounter_);
    if (buf.empty()) {
        Output::send<LogLevel::Error>(STR("[tcp] auth frame encode failed\n"));
        return;
    }
    const int sent = write_all(buf.data(), static_cast<int>(buf.size()));
    Output::send<LogLevel::Normal>(
        STR("[tcp] auth sent {:d}/{:d} bytes  ticket={:d}B\n"),
        sent, static_cast<int>(buf.size()), static_cast<int>(tickLen));
}

void TcpClient::send_join_request()
{
    Frame f;
    f.type         = MsgType::JoinRequest;
    f.connectionId = connId_;
    f.worldId      = worldId_;
    f.playerId     = playerId_;
    f.payload.assign(64, 0); // empty display name (server derives name from ticket)

    auto buf = frame_to_buf(f, seqCounter_, tickCounter_);
    if (!buf.empty()) write_all(buf.data(), static_cast<int>(buf.size()));
}

void TcpClient::send_heartbeat()
{
    Frame f;
    f.type         = MsgType::ClientHeartbeat;
    f.connectionId = connId_;
    f.sessionId    = sessionId_;
    f.worldId      = worldId_;
    f.playerId     = playerId_;
    f.entityId     = entityId_;

    auto buf = frame_to_buf(f, seqCounter_, tickCounter_);
    if (!buf.empty()) write_all(buf.data(), static_cast<int>(buf.size()));
}

void TcpClient::seed_game_session()
{
    // Push a synthetic PlayerConnected so dispatch_frame() in mod.cpp latches
    // the session (connectionId / sessionId / worldId / playerId / entityId).
    Frame f;
    f.type         = MsgType::PlayerConnected;
    f.connectionId = connId_;
    f.sessionId    = sessionId_;
    f.worldId      = worldId_;
    f.playerId     = playerId_;
    f.entityId     = entityId_;

    std::lock_guard<std::mutex> lk(inMtx_);
    inQueue_.push_back(std::move(f));
}

// ── Socket helpers ────────────────────────────────────────────────────────────

int TcpClient::write_all(const uint8_t* data, int len)
{
    int sent = 0;
    while (sent < len && !stopped_.load(std::memory_order_relaxed)) {
        if (sock_ == SDB_INVALID_SOCKET) break;
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock_, &wfds);
        struct timeval tv{};
        tv.tv_usec = static_cast<long>(SELECT_TIMEOUT_US);
        if (select(static_cast<int>(sock_) + 1, nullptr, &wfds, nullptr, &tv) <= 0)
            continue;
#ifdef _WIN32
        int n = send(sock_, reinterpret_cast<const char*>(data + sent), len - sent, 0);
        if (n == SOCKET_ERROR) break;
#else
        ssize_t n = send(sock_, data + sent, static_cast<size_t>(len - sent), 0);
        if (n < 0) break;
#endif
        sent += static_cast<int>(n);
    }
    return sent;
}

void TcpClient::drain_outbound()
{
    std::deque<std::vector<uint8_t>> local;
    {
        std::lock_guard<std::mutex> lk(outMtx_);
        local.swap(outQueue_);
    }
    for (auto& buf : local) {
        if (!buf.empty() && !killConn_)
            write_all(buf.data(), static_cast<int>(buf.size()));
    }
}

void TcpClient::close_socket()
{
    if (sock_ != SDB_INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sock_);
#else
        close(sock_);
#endif
        sock_ = SDB_INVALID_SOCKET;
    }
}

} // namespace sdb
