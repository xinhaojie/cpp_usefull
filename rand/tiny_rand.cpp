/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 17:58:04
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-18 18:12:12
* @FilePath     : /cpp_usefull/rand/tiny_rand.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_rand.hpp"
#include <iomanip>
#include <sstream>

namespace tiny_rand {
const std::string Random::charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

Random::Random() : gen(rd()), dis() {}

Random::Random(int min) : gen(rd()), dis(min, std::numeric_limits<int>::max()) {}

Random::Random(int min, int max) : gen(rd()), dis(min, max) {}

int Random::get_pseudo_random_number() {
    std::lock_guard<std::mutex> lock(mtx);
    return dis(gen);
}

int Random::get_true_random_number() {
    std::lock_guard<std::mutex> lock(mtx);
    return rd();
}

std::string Random::get_random_string(size_t length) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        std::uniform_int_distribution<> charDis(0, charset.size() - 1);
        result += charset[charDis(gen)];
    }

    return result;
}

std::string Random::generate_uuid() {
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << std::setw(8) << (microseconds & 0xFFFFFFFF);
    oss << '-';
    oss << std::setw(4) << ((microseconds >> 32) & 0xFFFF);
    oss << '-';
    oss << std::setw(4) << (((microseconds >> 48) & 0x0FFF) | 0x4000); // 设置版本为 4
    oss << '-';
    oss << std::setw(4) << ((rd() & 0x3FFF) | 0x8000); // 设置变体为 RFC 4122
    oss << '-';
    oss << std::setw(12) << (rd() & 0xFFFFFFFFFFFF);

    return oss.str();
}

}