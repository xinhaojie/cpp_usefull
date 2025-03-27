#pragma once

#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <zlib.h>
#include <cstdarg>

namespace fs = std::filesystem;

/* 库采用默认参数
    日志路径 ./logs/log
    最大文件大小5MB
    默认保留回滚数量 5
    默认开启压缩
    默认输出到日志文件
    不输出到控制台
    默认日志等级ERROR
    xinhaojie 25-03-13 11:52:24*/

// 定义日志级别枚举
enum class log_level {
    _DEBUG,
    _INFO,
    _WARN,
    _ERROR
};

class logger {
public:
    static logger& get_instance(std::string log_path = "/var/log/tiny_log",
                                size_t max_size = (5 * 1024 * 1024),
                                uint32_t file_counts = 5,
                                bool compress = true,
                                bool output_to_file = true,
                                bool output_to_stdout = false,
                                log_level log_level = log_level::_INFO);      // 获取单例实例.可以不传参使用默认参数

    class log_stream {
    public:
        log_stream(logger& logger, log_level level, const std::string& file, int line);
        ~log_stream();

        template<typename T>
        log_stream& operator<<(const T& value){
            oss_ << value;
            return *this;
        }

    private:
        logger& logger_;
        log_level level_;
        std::string file_;
        int line_;
        std::ostringstream oss_;
    };

    log_stream debug(const std::string& file, int line);
    log_stream info(const std::string& file, int line);
    log_stream warn(const std::string& file, int line);
    log_stream error(const std::string& file, int line);

    void logf(log_level level, const std::string& file, int line, const char* format, ...);

private:
    logger(const std::string& base_filename, size_t max_size, uint32_t file_counts,
        bool compress, bool output_to_file, bool output_to_stdout, log_level log_level);
    ~logger();

    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    void create_log_directory();
    void open_log_file();
    void roll_over();
    void rename_rolled_logs();
    void compress_and_rename(const std::string& input_filename, const std::string& output_filename);
    void cleanup_old_logs();
    std::string get_current_time_with_milliseconds();
    std::string get_timestamp();

    template<typename T>
    void log(log_level level, const std::string& file, int line, const T& value);

    std::string base_filename_;
    size_t max_size_;
    size_t current_size_ = 0;
    std::ofstream log_file_;
    std::mutex mtx_;
    // 日志参数
    uint32_t file_counts_;
    bool compress_;
    bool output_to_file_ ;
    bool output_to_stdout_;
    log_level log_level_;
};

// 方便使用的宏定义
#define LOG_DEBUG(...) logger::get_instance().debug(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_INFO(...) logger::get_instance().info(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_WARN(...) logger::get_instance().warn(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_ERROR(...) logger::get_instance().error(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_DEBUG_F(fmt, ...) logger::get_instance().logf(log_level::_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO_F(fmt, ...) logger::get_instance().logf(log_level::_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN_F(fmt, ...) logger::get_instance().logf(log_level::_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR_F(fmt, ...) logger::get_instance().logf(log_level::_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

//可以在程序运行前调用修改日志参数，不修改则使用默认值
#define LOG_ARGS(path, size, file_counts, compress, output_to_file, output_to_stdout, log_level) logger::get_instance(path, size, file_counts, compress, output_to_file, output_to_stdout, log_level)
