#include "tiny_system.hpp"
#include <iostream>
#include <thread>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

namespace tiny_system {

int System::get_cpu_num() {
    return std::thread::hardware_concurrency();
}

std::string System::get_current_working_directory() {
    return std::filesystem::current_path().string();
}

void System::daemonize() {
    if (::daemon(0, 0) == -1) {
        std::cerr << "Failed to daemonize the process." << std::endl;
    }
}

std::string System::get_environment_variable(const char* name) {
    const char* value = std::getenv(name);
    return value ? std::string(value) : "";
}

bool System::set_environment_variable(const char* name, const char* value) {
    return setenv(name, value, 1) == 0;
}

std::string System::get_executable_name() {
    std::string path = std::filesystem::read_symlink("/proc/self/exe").string();
    size_t pos = path.find_last_of('/');
    return (pos != std::string::npos) ? path.substr(pos + 1) : path;
}

std::string System::get_executable_path() {
    return std::filesystem::read_symlink("/proc/self/exe").string();
}

std::string System::get_home_directory() {
    const char* home = std::getenv("HOME");
    return home ? std::string(home) : "";
}

int System::get_process_id() {
    return static_cast<int>(::getpid());
}

std::vector<std::string> System::get_ip_addresses(const std::string& domain) {
    std::vector<std::string> ip_addresses;
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(domain.c_str(), NULL, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return ip_addresses;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        void* addr;
        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        ip_addresses.emplace_back(ipstr);
    }

    freeaddrinfo(res);
    return ip_addresses;
}

void System::set_process_title(char** argv, const char* new_name) {
    int size = 0;
    int i;
    // 申请新的空间存放 environ 中内容
    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }
    char* p = reinterpret_cast<char*>(malloc(size));
    char* last_argv = argv[0];
    for (i = 0; argv[i]; i++) {
        if (last_argv == argv[i]) {
            last_argv = argv[i] + strlen(argv[i]) + 1;
        }
    }
    for (i = 0; environ[i]; i++) {
        if (last_argv == environ[i]) {
            size = strlen(environ[i]) + 1;
            last_argv = environ[i] + size;
            memcpy(p, environ[i], size);
            environ[i] = reinterpret_cast<char*>(p);
            p += size;
        }
    }
    last_argv--;
    // 修改 argv[0]，argv剩余的空间全部填0
    strncpy(argv[0], new_name, last_argv - argv[0]);
    p = argv[0] + strlen(argv[0]) + 1;
    if (last_argv - p > 0) {
        memset(p, 0, last_argv - p);
    }
}

System::sig_handler_t System::set_signal_handler(int sig, sig_handler_t handler, int flag) {
    struct sigaction sa, old_sa;
    // 定义一个静态的 std::function 来存储当前的信号处理函数
    static std::function<void(int)> current_handler;
    // 将静态的 std::function 包装成普通函数指针
    sa.sa_handler = [](int s) {
        if (current_handler) {
            current_handler(s);
        }
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flag;
    // 获取旧的信号处理函数
    if (sigaction(sig, &sa, &old_sa) == -1) {
        std::cerr << "sigaction error" << std::endl;
        return nullptr;
    }
    // 保存旧的信号处理函数到一个 lambda 表达式中
    auto old_handler = [old_sa](int s) {
        if (old_sa.sa_handler) {
            old_sa.sa_handler(s);
        }
    };
    // 更新当前的信号处理函数
    current_handler = handler;
    // 再次调用 sigaction 设置新的信号处理函数
    if (sigaction(sig, &sa, nullptr) == -1) {
        std::cerr << "sigaction error" << std::endl;
        return nullptr;
    }
    return old_handler;
}

} // namespace tiny_system