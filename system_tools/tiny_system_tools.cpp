#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tiny_system_tools{

void LinuxSystemTools::set_proctitle(char** argv, const char* new_name)
{
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

std::string LinuxSystemTools::get_ip_by_name(char *host_name) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET_ADDRSTRLEN];

    // 初始化hints结构体
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // 只获取IPv4地址
    hints.ai_socktype = SOCK_STREAM;

    // 调用getaddrinfo函数进行域名解析
    if ((status = getaddrinfo(host_name, NULL, &hints, &res)) != 0) {
        error("getaddrinfo: %s", gai_strerror(status));
        return "";
    }

    // 遍历结果列表
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);

        // 将二进制IP地址转换为点分十进制字符串
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    }

    // 释放地址信息链表
    freeaddrinfo(res);

    return ipstr;
}

}