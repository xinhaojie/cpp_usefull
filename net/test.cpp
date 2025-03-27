#include <iostream>
#include <thread>
#include <chrono>
#include "tiny_net.hpp"

using network_utils::NetServer;

// TCP 服务器处理函数
void tcp_server_task() {
    auto server = network_utils::NetServer::create_tcp_server("127.0.0.1", 8080);
    if (!server) {
        std::cerr << "Failed to create TCP server." << std::endl;
        return;
    }
    std::cout << "TCP server is listening on port 8080..." << std::endl;

    auto client_fd_opt = server->accept();
    if (!client_fd_opt) {
        std::cerr << "Failed to accept TCP client connection." << std::endl;
        return;
    }

    auto client_socket = std::make_unique<network_utils::TcpSocket>();
    client_socket->set_socket(*client_fd_opt);

    auto data = client_socket->recv(1024);
    if (data) {
        std::cout << "TCP Server received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;

        std::vector<unsigned char> response = {'H', 'e', 'l', 'l', 'o', ' ', 'C', 'l', 'i', 'e', 'n', 't', '!'};
        client_socket->send(response);
    }
}

// TCP 客户端处理函数
void tcp_client_task() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto client = network_utils::NetClient::create_tcp_client("127.0.0.1", 8080);
    if (!client) {
        std::cerr << "Failed to create TCP client." << std::endl;
        return;
    }

    std::vector<unsigned char> message = {'H', 'e', 'l', 'l', 'o', ' ', 'S', 'e', 'r', 'v', 'e', 'r', '!'};
    client->send(message);

    auto data = client->recv(1024);
    if (data) {
        std::cout << "TCP Client received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;
    }
}

// UDP 服务器处理函数
void udp_server_task() {
    auto server = network_utils::NetServer::create_udp_server("127.0.0.1", 8081);
    if (!server) {
        std::cerr << "Failed to create UDP server." << std::endl;
        return;
    }
    std::cout << "UDP server is listening on port 8081..." << std::endl;

    auto data = server->recv(1024);
    if (data) {
        std::cout << "UDP Server received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;

        std::vector<unsigned char> response = {'H', 'e', 'l', 'l', 'o', ' ', 'C', 'l', 'i', 'e', 'n', 't', '!'};
        server->send(response);
    }
}

// UDP 客户端处理函数
void udp_client_task() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto client = network_utils::NetClient::create_udp_client("127.0.0.1", 8081);
    if (!client) {
        std::cerr << "Failed to create UDP client." << std::endl;
        return;
    }

    std::vector<unsigned char> message = {'H', 'e', 'l', 'l', 'o', ' ', 'S', 'e', 'r', 'v', 'e', 'r', '!'};
    client->send(message);

    auto data = client->recv(1024);
    if (data) {
        std::cout << "UDP Client received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;
    }
}

// 域套接字服务器处理函数
void DomainSocket_server_task(std::string path) {
    auto server = network_utils::NetServer::create_DomainSocket_server(path, true);
    if (!server) {
        std::cerr << "Failed to create domain socket server." << std::endl;
        return;
    }
    std::cout << "Domain socket server is listening on "<< path <<"..." << std::endl;

    auto client_fd_opt = server->accept();
    if (!client_fd_opt) {
        std::cerr << "Failed "<< path <<" to accept domain socket client connection." << std::endl;
        return;
    }

    auto client_socket = std::make_unique<network_utils::DomainSocket>();
    client_socket->set_socket(*client_fd_opt);

    auto data = client_socket->recv(1024);
    if (data) {
        std::cout << "Domain "<< path <<" Socket Server received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;

        std::vector<unsigned char> response = {'H', 'e', 'l', 'l', 'o', ' ', 'C', 'l', 'i', 'e', 'n', 't', '!'};
        client_socket->send(response);
    }
}

// 域套接字客户端处理函数
void DomainSocket_client_task(std::string path) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto client = network_utils::NetClient::create_DomainSocket_client(path);
    if (!client) {
        std::cerr << "Failed "<< path <<" to create domain socket client." << std::endl;
        return;
    }

    std::vector<unsigned char> message = {'H', 'e', 'l', 'l', 'o', ' ', 'S', 'e', 'r', 'v', 'e', 'r', '!'};
    client->send(message);

    auto data = client->recv(1024);
    if (data) {
        std::cout << "Domain "<< path <<" Socket Client received: ";
        for (unsigned char c : *data) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;
    }
}

void epoll_server_task() {
    // 创建 TCP 服务器套接字
    auto server = network_utils::NetServer::create_tcp_server("127.0.0.1", 8080);
    if (!server) {
        std::cerr << "Failed to create TCP server." << std::endl;
        return;
    }
    std::cout << "TCP server is listening on port 8080..." << std::endl;

    auto epoll = network_utils::EpollMonitor();
    epoll.add_socket(server->get_socket(), EPOLLIN);
    // 等待客户端连接
    while (1)
    {
        auto events = epoll.wait(-1);
        for(auto event : events){
            if(event.data.fd == server->get_socket()){
                auto client_fd_opt = server->accept();
                if (!client_fd_opt) {
                    std::cerr << "Failed to accept TCP client connection." << std::endl;
                    return;
                }
                std::cout << "TCP client connected." << std::endl;
                epoll.add_socket(*client_fd_opt, EPOLLIN);
            } else {
                int fd = event.data.fd;
                auto client_socket = std::make_unique<network_utils::TcpSocket>(fd);
                auto data = client_socket->recv(1024);
                if (data) {
                    std::cout << "TCP Server received: ";
                    if (data->size() > 0) {
                        for (unsigned char c : *data) {
                            std::cout << static_cast<char>(c);
                        }
                        std::cout << std::endl;
                        std::vector<unsigned char> response = {'G', 'o', 't', ' ', 'i', 't', '!'};
                        client_socket->send(response, 3);
                        client_socket->set_socket(-1);
                        std::cout << "TCP Server resp done" << std::endl;
                    }else {
                        std::cout << "TCP client lost connection." << std::endl;
                        epoll.remove_socket(event.data.fd);
                        close(event.data.fd);
                    }
                } else {
                    std::cout << "TCP client lost connection." << std::endl;
                    epoll.remove_socket(event.data.fd);
                    close(event.data.fd);
                }
            }
        }
    }

}

int main() {
    // 启动 TCP 服务器和客户端线程
    // std::thread tcp_server(tcp_server_task);
    // std::thread tcp_client(tcp_client_task);

    // // 启动 UDP 服务器和客户端线程
    // std::thread udp_server(udp_server_task);
    // std::thread udp_client(udp_client_task);

    // 启动域套接字服务器和客户端线程
    std::thread domain_server1(epoll_server_task);

    // 等待所有线程完成
    // tcp_server.join();
    // tcp_client.join();
    // udp_server.join();
    // udp_client.join();
    domain_server1.join();
    return 0;
}