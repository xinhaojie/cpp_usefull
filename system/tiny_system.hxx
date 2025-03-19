#pragma once
#include <string>
#include <functional>
#include <vector>

namespace tiny_system {

class System {
public:
    // 返回系统 CPU 核数
    static int get_cpu_num();
    // 返回当前工作目录
    static std::string get_current_working_directory();
    // 将当前进程放到后台运行
    static void daemonize();
    // 获取系统环境变量的值
    static std::string get_environment_variable(const char* name);
    // 设置环境变量的值，成功时返回 true，否则返回 false
    static bool set_environment_variable(const char* name, const char* value);
    // 返回当前进程名，不含路径
    static std::string get_executable_name();
    // 返回当前进程的完整路径
    static std::string get_executable_path();
    // 返回当前用户的 home 目录
    static std::string get_home_directory();
    // 返回当前进程的 id
    static int get_process_id();
    // 通过域名获取 IP 地址
    static std::vector<std::string> get_ip_addresses(const std::string& domain);
    // 设置进程名称
    static void set_process_title(char** argv, const char* new_name);

    using sig_handler_t = std::function<void(int)>;
    // 设置信号处理函数
    static sig_handler_t set_signal_handler(int sig, sig_handler_t handler, int flag = 0);
};

} // namespace tiny_system    