/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 18:04:00
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-18 18:09:50
* @FilePath     : /cpp_usefull/rand/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_rand.hpp"
#include <iostream>

using tiny_rand::Random;
int main() {
    // 不设置范围
    Random rng1;
    std::cout << "不设置范围 - 伪随机数: " << rng1.get_pseudo_random_number() << std::endl;

    // 只设置最小值
    Random rng2(10);
    std::cout << "只设置最小值 - 伪随机数: " << rng2.get_pseudo_random_number() << std::endl;

    // 设置最小值和最大值
    Random rng3(10, 20);
    std::cout << "设置最小值和最大值 - 伪随机数: " << rng3.get_pseudo_random_number() << std::endl;

    std::cout << "真随机数: " << rng1.get_true_random_number() << std::endl;
    std::cout << "随机字符串: " << rng1.get_random_string(10) << std::endl;
    std::cout << "UUID: " << rng1.generate_uuid() << std::endl;

    return 0;
}