#include "tiny_logger.hpp"
#include <chrono>
#include <thread>

int main() {
    LOG_ARGS("./logs/log", 1024 * 1024, 5, true, true, true, log_level::_DEBUG);
    uint32_t i = 0;
    while (true) {
        // 输出不同级别的日志
        LOG_DEBUG("This is a debug log message.");
        LOG_INFO("This is an info log message." << i);
        LOG_WARN("This is a warning log message." << i);
        LOG_ERROR("This is an error log message." << i);
        i++;
        printf("This is a printf log message\n");
        LOG_DEBUG_F("This is a debug log message.");

        // 为了避免日志输出过快，可以适当休眠一段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}