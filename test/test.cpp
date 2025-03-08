#include "logger.hxx"
#include <chrono>
#include <thread>

int main() {
    while (true) {
        // 输出不同级别的日志
        LOG_DBG("This is a debug log message.");
        LOG_INFO("This is an info log message.");
        LOG_WARN("This is a warning log message.");
        LOG_ERROR("This is an error log message.");

        // 为了避免日志输出过快，可以适当休眠一段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}