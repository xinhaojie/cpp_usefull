#include "tiny_net.hxx"

namespace network_utils {

// TcpSocket 实现
TcpSocket::TcpSocket() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ == -1) {
        throw std::system_error(errno, std::generic_category(), "Failed to create TCP socket");
    }
    int opt = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    self_crated_fd_ = true;
}

TcpSocket::TcpSocket(int sockfd) : sockfd_(sockfd) {}

TcpSocket::~TcpSocket() {
    if (self_crated_fd_ && sockfd_ != -1) {
        close(sockfd_);
    }
}

bool TcpSocket::bind(const std::string& address, int port) {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(address.empty() ? nullptr : address.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        return false;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        if (::bind(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (p == nullptr) {
        return false;
    }
    return true;
}

bool TcpSocket::listen(int backlog) {
    return ::listen(sockfd_, backlog) != -1;
}

std::optional<int> TcpSocket::accept() {
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = ::accept(sockfd_, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
    if (client_fd == -1) {
        return std::nullopt;
    }
    return client_fd;
}

bool TcpSocket::connect(const std::string& address, int port) {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(address.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        return false;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        if (::connect(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (p == nullptr) {
        return false;
    }
    return true;
}

ssize_t TcpSocket::send(const std::vector<unsigned char>& data, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }
    return ::send(sockfd_, data.data(), data.size(), 0);
}

std::optional<std::vector<unsigned char>> TcpSocket::recv(size_t buffer_size, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    std::vector<unsigned char> buffer(buffer_size);
    ssize_t bytes_received = ::recv(sockfd_, buffer.data(), buffer_size, 0);
    if (bytes_received == -1) {
        return std::nullopt;
    }
    buffer.resize(bytes_received);
    return buffer;
}

int TcpSocket::get_socket() const {
    return sockfd_;
}

void TcpSocket::set_socket(int fd) {
    sockfd_ = fd;
}

// UdpSocket 实现
UdpSocket::UdpSocket() {
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ == -1) {
        throw std::system_error(errno, std::generic_category(), "Failed to create UDP socket");
    }
    int opt = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    self_crated_fd_ = true;
}

UdpSocket::UdpSocket(int sockfd) : sockfd_(sockfd) {}

UdpSocket::~UdpSocket() {
    if (self_crated_fd_ && sockfd_ != -1) {
        close(sockfd_);
    }
}

bool UdpSocket::bind(const std::string& address, int port) {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(address.empty() ? nullptr : address.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        return false;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        if (::bind(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (p == nullptr) {
        return false;
    }
    return true;
}

bool UdpSocket::listen(int backlog) {
    // UDP 不需要监听
    return true;
}

std::optional<int> UdpSocket::accept() {
    // UDP 没有连接接受的概念
    return std::nullopt;
}

bool UdpSocket::connect(const std::string& address, int port) {
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(address.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        return false;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        if (::connect(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (p == nullptr) {
        return false;
    }
    return true;
}

ssize_t UdpSocket::send(const std::vector<unsigned char>& data, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }
    return ::send(sockfd_, data.data(), data.size(), 0);
}

std::optional<std::vector<unsigned char>> UdpSocket::recv(size_t buffer_size, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    std::vector<unsigned char> buffer(buffer_size);
    ssize_t bytes_received = ::recv(sockfd_, buffer.data(), buffer_size, 0);
    if (bytes_received == -1) {
        return std::nullopt;
    }
    buffer.resize(bytes_received);
    return buffer;
}

int UdpSocket::get_socket() const {
    return sockfd_;
}

void UdpSocket::set_socket(int fd) {
    sockfd_ = fd;
}

// DomainSocket 实现
DomainSocket::DomainSocket() {
    sockfd_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd_ == -1) {
        throw std::system_error(errno, std::generic_category(), "Failed to create domain socket");
    }
    self_crated_fd_ = true;
}

DomainSocket::DomainSocket(int sockfd) : sockfd_(sockfd) {}

DomainSocket::~DomainSocket() {
    if (self_crated_fd_ && sockfd_ != -1) {
        close(sockfd_);
    }
}

bool DomainSocket::bind(const std::string& path, int) {
    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, path.c_str(), sizeof(server_addr.sun_path) - 1);
    if (path[0] == '/'){
        unlink(path.c_str());  // 移除可能存在的旧套接字文件
    } else {
        server_addr.sun_path[0] = '\0';
    }
    return ::bind(sockfd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) != -1;
}

bool DomainSocket::listen(int backlog) {
    return ::listen(sockfd_, backlog) != -1;
}

std::optional<int> DomainSocket::accept() {
    sockaddr_un client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = ::accept(sockfd_, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
    if (client_fd == -1) {
        return std::nullopt;
    }
    return client_fd;
}

bool DomainSocket::connect(const std::string& path, int) {
    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, path.c_str(), sizeof(server_addr.sun_path) - 1);
    if (path[0] != '/'){
        server_addr.sun_path[0] = '\0';
    }
    return ::connect(sockfd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) != -1;
}

ssize_t DomainSocket::send(const std::vector<unsigned char>& data, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }
    return ::send(sockfd_, data.data(), data.size(), 0);
}

std::optional<std::vector<unsigned char>> DomainSocket::recv(size_t buffer_size, int timeout_ms) {
    if (timeout_ms > 0) {
        timeval timeout{};
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    std::vector<unsigned char> buffer(buffer_size);
    ssize_t bytes_received = ::recv(sockfd_, buffer.data(), buffer_size, 0);
    if (bytes_received == -1) {
        return std::nullopt;
    }
    buffer.resize(bytes_received);
    return buffer;
}

int DomainSocket::get_socket() const {
    return sockfd_;
}

void DomainSocket::set_socket(int fd) {
    sockfd_ = fd;
}

// NetServer 实现
std::unique_ptr<NetworkSocket> NetServer::create_tcp_server(const std::string& address, int port, int backlog) {
    auto server = std::make_unique<TcpSocket>();
    if (!server->bind(address, port)) {
        return nullptr;
    }
    if (!server->listen(backlog)) {
        return nullptr;
    }
    return server;
}

std::unique_ptr<NetworkSocket> NetServer::create_udp_server(const std::string& address, int port) {
    auto server = std::make_unique<UdpSocket>();
    if (!server->bind(address, port)) {
        return nullptr;
    }
    return server;
}

std::unique_ptr<NetworkSocket> NetServer::create_DomainSocket_server(const std::string& path, int backlog) {
    auto server = std::make_unique<DomainSocket>();
    std::cout << "path: " << path << std::endl;
    if (!server->bind(path, 0)) {
        perror("bind");
        return nullptr;
    }
    if (!server->listen(backlog)) {
        return nullptr;
    }
    return server;
}

// NetClient 实现
std::unique_ptr<NetworkSocket> NetClient::create_tcp_client(const std::string& address, int port) {
    auto client = std::make_unique<TcpSocket>();
    if (!client->connect(address, port)) {
        return nullptr;
    }
    return client;
}

std::unique_ptr<NetworkSocket> NetClient::create_udp_client(const std::string& address, int port) {
    auto client = std::make_unique<UdpSocket>();
    if (!client->connect(address, port)) {
        return nullptr;
    }
    return client;
}

std::unique_ptr<NetworkSocket> NetClient::create_DomainSocket_client(const std::string& path) {
    auto client = std::make_unique<DomainSocket>();
    if (!client->connect(path, 0)) {
        perror("connect");
        return nullptr;
    }
    return client;
}

// EpollMonitor 实现
EpollMonitor::EpollMonitor() {
    epollfd_ = epoll_create1(0);
    if (epollfd_ == -1) {
        throw std::system_error(errno, std::generic_category(), "Failed to create epoll instance");
    }
}

EpollMonitor::~EpollMonitor() {
    if (epollfd_ != -1) {
        close(epollfd_);
    }
}

bool EpollMonitor::add_socket(int fd, uint32_t events) {
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) != -1;
}

bool EpollMonitor::remove_socket(int fd) {
    return epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) != -1;
}

std::vector<epoll_event> EpollMonitor::wait(int timeout_ms) {
    std::vector<epoll_event> events(10);
    int num_events = epoll_wait(epollfd_, events.data(), events.size(), timeout_ms);
    if (num_events == -1) {
        return {};
    }
    events.resize(num_events);
    return events;
}

}  // namespace network_utils
