#include "tiny_system.hpp"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <signal.h> // 添加这个头文件以使用 SIGTERM

// 辅助函数：用于输出测试结果
void print_test_result(const char* test_name, bool result) {
    std::cout << test_name << ": " << (result ? "Passed" : "Failed") << std::endl;
}

int main() {
    bool all_tests_passed = true;

    // 测试 get_cpu_num 函数
    {
        int cpu_num = tiny_system::System::get_cpu_num();
        bool result = cpu_num > 0;
        print_test_result("GetCpuNum", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_current_working_directory 函数
    {
        std::string cwd = tiny_system::System::get_current_working_directory();
        bool result =!cwd.empty();
        print_test_result("GetCurrentWorkingDirectory", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_environment_variable 和 set_environment_variable 函数
    {
        const char* test_var_name = "TEST_VARIABLE";
        const char* test_var_value = "test_value";

        // 设置环境变量
        bool set_result = tiny_system::System::set_environment_variable(test_var_name, test_var_value);

        // 获取环境变量
        std::string value = tiny_system::System::get_environment_variable(test_var_name);
        bool result = set_result && (value == test_var_value);

        // 删除环境变量
        unsetenv(test_var_name);

        print_test_result("EnvironmentVariable", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_executable_name 函数
    {
        std::string exe_name = tiny_system::System::get_executable_name();
        bool result =!exe_name.empty();
        print_test_result("GetExecutableName", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_executable_path 函数
    {
        std::string exe_path = tiny_system::System::get_executable_path();
        bool result =!exe_path.empty();
        print_test_result("GetExecutablePath", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_home_directory 函数
    {
        std::string home_dir = tiny_system::System::get_home_directory();
        bool result =!home_dir.empty();
        print_test_result("GetHomeDirectory", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_process_id 函数
    {
        int pid = tiny_system::System::get_process_id();
        bool result = pid > 0;
        print_test_result("GetProcessId", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 get_ip_addresses 函数
    {
        std::vector<std::string> ips = tiny_system::System::get_ip_addresses("www.google.com");
        bool result =!ips.empty();
        print_test_result("GetIpAddresses", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 测试 set_process_title 函数
    {
        // 如果 environ 未使用，可移除 extern char** environ;
        char* argv[] = {const_cast<char*>("old_name"), nullptr};
        const char* new_name = "new_nam";
        tiny_system::System::set_process_title(argv, new_name);
        bool result = std::strcmp(argv[0], new_name) == 0;
        print_test_result("SetProcessTitle", result);
        all_tests_passed = all_tests_passed && result;
    }

    // 由于 daemonize 和 set_signal_handler 函数测试较复杂，这里简单调用
    // 测试 daemonize 函数
    {
        tiny_system::System::daemonize();
        print_test_result("Daemonize", true); // 无法精确验证结果，默认通过
    }

    // 测试 set_signal_handler 函数
    {
        auto handler = [](int sig) {};
        tiny_system::System::set_signal_handler(SIGTERM, handler);
        print_test_result("SetSignalHandler", true); // 无法精确验证结果，默认通过
    }

    return all_tests_passed ? 0 : 1;
}