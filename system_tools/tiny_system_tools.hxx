#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace tiny_system_tools{

class TinySystemTools{
    public:
        virtual ~tiny_system_tools(){} = default;
        virtual static uint64_t get_system_time() = 0;
        virtual static void set_proctitle(char** argv, const char* new_name) = 0;
        virtual static std::string get_ip_by_name(char *host_name) = 0;
}

class LinuxSystemTools : public TinySystemTools{
    public:
        LinuxSystemTools(){}
        ~LinuxSystemTools(){}
        static uint64_t get_system_time() override;
        static void set_proctitle(char** argv, const char* new_name) override;
        static std::string get_ip_by_name(char *host_name) override;
}


}