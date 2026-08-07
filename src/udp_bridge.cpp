#include "udp_bridge.hpp"
#include <cstring>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  pragma comment(lib, "Ws2_32.lib")
static void init_winsock() {
    static bool done = false;
    if (!done) { WSADATA d; WSAStartup(MAKEWORD(2, 2), &d); done = true; }
}
static void set_nonblocking(SOCKET s) {
    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);
}
static bool would_block() {
    return WSAGetLastError() == WSAEWOULDBLOCK;
}
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
static void init_winsock() {}
static void set_nonblocking(int s) {
    int flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);
}
static bool would_block() {
    return errno == EAGAIN || errno == EWOULDBLOCK;
}
#endif

namespace sdb {

bool UdpBridge::open(uint16_t bind_port, uint16_t runtime_port)
{
    init_winsock();

    sock_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ == SDB_INVALID_SOCKET)
        return false;

    set_nonblocking(sock_);

    sockaddr_in local{};
    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    local.sin_port        = htons(bind_port);

    if (::bind(sock_, reinterpret_cast<sockaddr*>(&local), sizeof(local)) != 0) {
        shutdown();
        return false;
    }

    runtime_port_ = runtime_port;
    return true;
}

bool UdpBridge::send_raw(const uint8_t* data, int len)
{
    if (sock_ == SDB_INVALID_SOCKET || len <= 0)
        return false;

    sockaddr_in dst{};
    dst.sin_family      = AF_INET;
    dst.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    dst.sin_port        = htons(runtime_port_);

    return ::sendto(sock_,
                    reinterpret_cast<const char*>(data), len,
                    0,
                    reinterpret_cast<sockaddr*>(&dst), sizeof(dst))
           == len;
}

std::vector<Frame> UdpBridge::recv_all()
{
    std::vector<Frame> out;
    if (sock_ == SDB_INVALID_SOCKET)
        return out;

    sockaddr_in src{};
    socklen_t   src_len = sizeof(src);

    while (true) {
#ifdef _WIN32
        const int n = ::recvfrom(sock_,
                                 reinterpret_cast<char*>(recv_buf_),
                                 static_cast<int>(sizeof(recv_buf_)),
                                 0,
                                 reinterpret_cast<sockaddr*>(&src),
                                 reinterpret_cast<int*>(&src_len));
#else
        const ssize_t n = ::recvfrom(sock_,
                                     recv_buf_, sizeof(recv_buf_),
                                     0,
                                     reinterpret_cast<sockaddr*>(&src),
                                     &src_len);
#endif
        if (n <= 0) {
            if (would_block()) break;
            break;
        }

        // Accept only loopback source from the runtime port
        if (ntohl(src.sin_addr.s_addr) != INADDR_LOOPBACK) continue;
        if (ntohs(src.sin_port)        != runtime_port_)    continue;

        if (auto f = decode_frame(recv_buf_, static_cast<int>(n)))
            out.push_back(std::move(*f));
    }
    return out;
}

void UdpBridge::shutdown()
{
    if (sock_ == SDB_INVALID_SOCKET) return;
#ifdef _WIN32
    ::closesocket(sock_);
#else
    ::close(sock_);
#endif
    sock_ = SDB_INVALID_SOCKET;
}

} // namespace sdb
