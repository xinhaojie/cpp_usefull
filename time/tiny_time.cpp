/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 17:22:45
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-18 17:43:48
* @FilePath     : /cpp_usefull/time/tiny_time.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/

#include "tiny_time.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <ctime>

namespace tiny_time {
uint64_t Time::sec_epoch() {
    auto now = std::chrono::system_clock::now();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());
}

uint64_t Time::ms_epoch() {
    auto now = std::chrono::system_clock::now();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}

uint64_t Time::us_epoch() {
    auto now = std::chrono::system_clock::now();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
}

uint64_t Time::sec_up() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_sec);
}

uint64_t Time::ms_up() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

uint64_t Time::us_up() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

std::string Time::get_str(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(now_tm, format.c_str());
    return oss.str();
}

void Time::sleep_sec(uint64_t seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Time::sleep_ms(uint64_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Time::sleep_us(uint64_t microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

Time::Timer::Timer(bool auto_start) : running(false), start_time() {
    if (auto_start) {
        start();
    }
}

void Time::Timer::start() {
    if (!running) {
        running = true;
        start_time = std::chrono::high_resolution_clock::now();
    }
}

void Time::Timer::stop() {
    running = false;
}

void Time::Timer::reset() {
    if (running) {
        start_time = std::chrono::high_resolution_clock::now();
    }
}

uint64_t Time::Timer::get_s() const {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count());
    }
    return 0;
}

uint64_t Time::Timer::get_ms() const {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count());
    }
    return 0;
}

uint64_t Time::Timer::get_us() const {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count());
    }
    return 0;
}
}