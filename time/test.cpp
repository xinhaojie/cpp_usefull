/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 17:40:33
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-27 14:10:36
* @FilePath     : /cpp_usefull/time/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_time.hpp"
#include <iostream>

using tiny_time::Time;

int main() {
    // 输出从 1970 年 1 月 1 日以来的秒数
    std::cout << "Seconds since epoch: " << Time::sec_since_epoch() << std::endl;
    // 输出从 1970 年 1 月 1 日以来的毫秒数
    std::cout << "Milliseconds since epoch: " << Time::ms_since_epoch() << std::endl;
    // 输出从 1970 年 1 月 1 日以来的微秒数
    std::cout << "Microseconds since epoch: " << Time::us_since_epoch() << std::endl;

    // 输出从系统启动后到现在的秒数
    std::cout << "Seconds since system startup: " << Time::sec_since_startup() << std::endl;
    // 输出从系统启动后到现在的毫秒数
    std::cout << "Milliseconds since system startup: " << Time::ms_since_startup() << std::endl;
    // 输出从系统启动后到现在的微秒数
    std::cout << "Microseconds since system startup: " << Time::us_since_startup() << std::endl;

    // 输出当前时间字符串，使用默认格式
    std::cout << "Current time (default format): " << Time::get_current_time_str() << std::endl;

    // 输出当前年份
    std::cout << "Current year: " << Time::get_current_time_str("%Y") << std::endl;

    // 输出当前小时
    std::cout << "Current hour: " << Time::get_current_time_str("%H") << std::endl;

    // 示例：睡眠 2 秒
    std::cout << "Going to sleep for 2 seconds..." << std::endl;
    Time::sleep_sec(2);
    std::cout << "Woke up after 2 seconds." << std::endl;

    // 定时器示例
    Time::Timer timer(true);
    Time::sleep_sec(3);
    std::cout << "Elapsed seconds: " << timer.get_elapsed_seconds() << std::endl;
    std::cout << "Elapsed milliseconds: " << timer.get_elapsed_milliseconds() << std::endl;
    std::cout << "Elapsed microseconds: " << timer.get_elapsed_microseconds() << std::endl;

    timer.reset();
    Time::sleep_sec(2);
    std::cout << "Elapsed seconds after reset: " << timer.get_elapsed_seconds() << std::endl;

    timer.stop();
    std::cout << "Timer stopped. Elapsed seconds: " << timer.get_elapsed_seconds() << std::endl;

    return 0;
}