#pragma once

#include <random>
#include <string>
#include <chrono>
#include <mutex>

namespace tiny_rand {
class Random {
private:
    std::random_device rd;
    std::mt19937 gen;
    static const std::string charset;
    std::mutex mtx;
    std::uniform_int_distribution<> dis;

public:
    Random();
    Random(int min);
    Random(int min, int max);

    // 获取伪随机数
    int get_pseudo_random_number();

    // 获取真随机数
    int get_true_random_number();

    // 生成随机字符串
    std::string get_random_string(size_t length);

    // 生成 UUID
    std::string generate_uuid();
};
}