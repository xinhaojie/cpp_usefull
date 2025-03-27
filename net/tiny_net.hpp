#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <system_error>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <chrono>

/* tips:域套接字创建接口，根据传入的字符串第一个字节进行判断，如果第一个字节是 / 则按照命名空间套接字创建
    如果第一个字节不是 / 则按照抽象命名空间创建，建议使用抽象命名空间 xinhaojie 25-03-13 16:08:41*/

namespace network_utils {

enum class SocketType {
    TCP,
    UDP,
    DomainSocket
};

class NetworkSocket {
public:
    virtual ~NetworkSocket() = default;
    virtual bool bind(const std::string& address, int port) = 0;
    virtual bool listen(int backlog) = 0;
    virtual std::optional<int> accept() = 0;
    virtual bool connect(const std::string& address, int port) = 0;
    virtual ssize_t send(const std::vector<unsigned char>& data, int timeout_ms = -1) = 0;
    virtual std::optional<std::vector<unsigned char>> recv(size_t buffer_size, int timeout_ms = -1) = 0;
    virtual int get_socket() const = 0;
    virtual void set_socket(int fd) = 0;
};

class TcpSocket : public NetworkSocket {
public:
    TcpSocket();
    TcpSocket(int fd);
    ~TcpSocket() override;
    bool bind(const std::string& address, int port) override;
    bool listen(int backlog) override;
    std::optional<int> accept() override;
    bool connect(const std::string& address, int port) override;
    ssize_t send(const std::vector<unsigned char>& data, int timeout_ms = -1) override;
    std::optional<std::vector<unsigned char>> recv(size_t buffer_size, int timeout_ms = -1) override;
    int get_socket() const override;
    void set_socket(int fd) override;

private:
    int sockfd_;
    bool self_crated_fd_ = false;               // 是否是自己创建的套接字
};

class UdpSocket : public NetworkSocket {
public:
    UdpSocket();
    UdpSocket(int fd);
    ~UdpSocket() override;
    bool bind(const std::string& address, int port) override;
    bool listen(int backlog) override;
    std::optional<int> accept() override;
    bool connect(const std::string& address, int port) override;
    ssize_t send(const std::vector<unsigned char>& data, int timeout_ms = -1) override;
    std::optional<std::vector<unsigned char>> recv(size_t buffer_size, int timeout_ms = -1) override;
    int get_socket() const override;
    void set_socket(int fd) override;

private:
    int sockfd_;
    bool self_crated_fd_ = false;               // 是否是自己创建的套接字
};

class DomainSocket : public NetworkSocket {
public:
    DomainSocket();
    DomainSocket(int fd);
    ~DomainSocket() override;
    bool bind(const std::string& path, int port = 0) override;
    bool listen(int backlog) override;
    std::optional<int> accept() override;
    bool connect(const std::string& path, int port = 0) override;
    ssize_t send(const std::vector<unsigned char>& data, int timeout_ms = -1) override;
    std::optional<std::vector<unsigned char>> recv(size_t buffer_size, int timeout_ms = -1) override;
    int get_socket() const override;
    void set_socket(int fd) override;

private:
    int sockfd_;
    bool self_crated_fd_ = false;               // 是否是自己创建的套接字
};

class NetServer {
public:
    static std::unique_ptr<NetworkSocket> create_tcp_server(const std::string& address, int port, int backlog = 5);
    static std::unique_ptr<NetworkSocket> create_udp_server(const std::string& address, int port);
    static std::unique_ptr<NetworkSocket> create_DomainSocket_server(const std::string& path, int backlog = 5);
};

class NetClient {
public:
    static std::unique_ptr<NetworkSocket> create_tcp_client(const std::string& address, int port);
    static std::unique_ptr<NetworkSocket> create_udp_client(const std::string& address, int port);
    static std::unique_ptr<NetworkSocket> create_DomainSocket_client(const std::string& path);
};

class EpollMonitor {
public:
    EpollMonitor();
    ~EpollMonitor();
    bool add_socket(int fd, uint32_t events);
    bool remove_socket(int fd);
    std::vector<epoll_event> wait(int timeout_ms);

private:
    int epollfd_;
};

}  // namespace network_utils
