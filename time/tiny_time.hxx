#pragma once

#include <chrono>
#include <ctime>
#include <cstdint>
#include <string>
#include <atomic>

namespace tiny_time {
class Time {
public:
    static uint64_t sec_epoch();
    static uint64_t ms_epoch();
    static uint64_t us_epoch();
    static uint64_t sec_up();
    static uint64_t ms_up();
    static uint64_t us_up();
    static std::string get_str(const std::string& format = "%Y-%m-%d %H:%M:%S");
    static void sleep_sec(uint64_t seconds);
    static void sleep_ms(uint64_t milliseconds);
    static void sleep_us(uint64_t microseconds);

    class Timer {
    public:
        Timer(bool auto_start = false);
        void start();
        void stop();
        void reset();
        uint64_t get_s() const;
        uint64_t get_ms() const;
        uint64_t get_us() const;

    private:
        std::atomic<bool> running;
        std::chrono::high_resolution_clock::time_point start_time;
    };
};
}