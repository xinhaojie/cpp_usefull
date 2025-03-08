/****************************************************************************************************
* @......................................&&.........................
* @....................................&&&..........................
* @.................................&&&&............................
* @...............................&&&&..............................
* @.............................&&&&&&..............................
* @...........................&&&&&&....&&&..&&&&&&&&&&&&&&&........
* @..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............
* @................&...&&&&&&&&&&&&&&&&&&&&&&&&&&&&.................
* @.......................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........
* @...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...............
* @..................&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
* @...............&&&&&@  &&&&&&&&&&..&&&&&&&&&&&&&&&&&&&...........
* @..............&&&&&&&&&&&&&&&.&&....&&&&&&&&&&&&&..&&&&&.........
* @..........&&&&&&&&&&&&&&&&&&...&.....&&&&&&&&&&&&&...&&&&........
* @........&&&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&....&&&.......
* @.......&&&&&&&&.....................&&&&&&&&&&&&&&&&.....&&......
* @........&&&&&.....................&&&&&&&&&&&&&&&&&&.............
* @..........&...................&&&&&&&&&&&&&&&&&&&&&&&............
* @................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
* @..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&..&&&&&............
* @..............&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&....&&&&&............
* @...........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&......&&&&............
* @.........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........&&&&............
* @.......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&............
* @......&&&&&&&&&&&&&&&&&&&...&&&&&&...............&&&.............
* @.....&&&&&&&&&&&&&&&&............................&&..............
* @....&&&&&&&&&&&&&&&.................&&...........................
* @...&&&&&&&&&&&&&&&.....................&&&&......................
* @...&&&&&&&&&&.&&&........................&&&&&...................
* @..&&&&&&&&&&&..&&..........................&&&&&&&...............
* @..&&&&&&&&&&&&...&............&&&.....&&&&...&&&&&&&.............
* @..&&&&&&&&&&&&&.................&&&.....&&&&&&&&&&&&&&...........
* @..&&&&&&&&&&&&&&&&..............&&&&&&&&&&&&&&&&&&&&&&&&.........
* @..&&.&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&&&&&&&&&&&&.......
* @...&&..&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&...&&&&&&&&&&&&......
* @....&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&&&&&.....
* @.......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............&&&&&&&....
* @.......&&&&&.&&&&&&&&&&&&&&&&&&..&&&&&&&&...&..........&&&&&&....
* @........&&&.....&&&&&&&&&&&&&.....&&&&&&&&&&...........&..&&&&...
* @.......&&&........&&&.&&&&&&&&&.....&&&&&.................&&&&...
* @.......&&&...............&&&&&&&.......&&&&&&&&............&&&...
* @........&&...................&&&&&&.........................&&&..
* @.........&.....................&&&&........................&&....
* @...............................&&&.......................&&......
* @................................&&......................&&.......
* @.................................&&..............................
* @..................................&..............................
* @
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-08 14:03:56
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-08 15:24:40
* @FilePath     : /cpp_usefull/include/logger.hxx
* @Description  : 辛豪杰自用的日志库，带上头文件
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/

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

// 定义日志文件名、路径和最大大小的宏
#define LOG_FILE_PATH "./logs/"
#define LOG_FILE_NAME "main"
#define MAX_LOG_FILE_SIZE (5 * 1024 * 1024) // 5MB
// 最多保留的压缩日志文件数量
#define MAX_ROLLED_LOG_FILES 5
// 定义是否压缩日志文件的宏
#define COMPRESS_LOG_FILES 1

// 定义日志级别枚举
enum class log_level {
    DBG,
    INFO,
    WARN,
    ERROR
};

class logger {
public:
    static logger& get_instance() {
        static logger instance(LOG_FILE_PATH + std::string(LOG_FILE_NAME), MAX_LOG_FILE_SIZE);
        return instance;
    }

    class log_stream {
    public:
        log_stream(logger& logger, log_level level, const std::string& file, int line)
            : logger_(logger), level_(level), file_(file), line_(line) {}

        ~log_stream() {
            logger_.log(level_, file_, line_, oss_.str());
        }

        template<typename T>
        log_stream& operator<<(const T& value) {
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

    log_stream dbg(const std::string& file, int line) { return log_stream(*this, log_level::DBG, file, line); }
    log_stream info(const std::string& file, int line) { return log_stream(*this, log_level::INFO, file, line); }
    log_stream warn(const std::string& file, int line) { return log_stream(*this, log_level::WARN, file, line); }
    log_stream error(const std::string& file, int line) { return log_stream(*this, log_level::ERROR, file, line); }

    void logf(log_level level, const std::string& file, int line, const char* format, ...) {
        std::va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        log(level, file, line, buffer);
    }

private:
    logger(const std::string& base_filename, size_t max_size)
        : base_filename_(base_filename), max_size_(max_size) {
        create_log_directory();
        open_log_file();
    }

    ~logger() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    void create_log_directory() {
        fs::path log_dir = fs::path(base_filename_).parent_path();
        if (!fs::exists(log_dir)) {
            fs::create_directories(log_dir);
        }
    }

    void open_log_file() {
        log_file_.open(base_filename_, std::ios::app);
        if (log_file_.is_open()) {
            current_size_ = fs::file_size(base_filename_);
        } else {
            std::cerr << "Failed to open log file: " << base_filename_ << std::endl;
        }
    }

    void roll_over() {
        if (log_file_.is_open()) {
            log_file_.close();
        }

        // 重命名所有已存在的压缩日志文件，序号加 1
        rename_rolled_logs();

        std::string file_ext = COMPRESS_LOG_FILES ? ".log.gz" : ".log";
        std::string rolled_filename = base_filename_ + ".0" + file_ext;
#if COMPRESS_LOG_FILES
        compress_and_rename(base_filename_, rolled_filename);
#else
        fs::rename(base_filename_, rolled_filename);
#endif

        // 重新打开新的日志文件
        open_log_file();

        // 清理多余的日志文件
        cleanup_old_logs();
    }

    void rename_rolled_logs() {
        std::vector<std::pair<int, std::string>> numbered_logs;
        std::string base_name = fs::path(base_filename_).filename().string();
        std::string file_ext = COMPRESS_LOG_FILES ? ".log.gz" : ".log";
        for (const auto& entry : fs::directory_iterator(fs::path(base_filename_).parent_path())) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                size_t pos1 = filename.find(base_name + ".");
                if (pos1 != std::string::npos) {
                    size_t pos2 = filename.find(file_ext, pos1 + base_name.length() + 1);
                    if (pos2 != std::string::npos) {
                        std::string num_str = filename.substr(pos1 + base_name.length() + 1, pos2 - (pos1 + base_name.length() + 1));
                        try {
                            int num = std::stoi(num_str);
                            numbered_logs.emplace_back(num, entry.path().string());
                        } catch (const std::invalid_argument&) {}
                    }
                }
            }
        }
        std::sort(numbered_logs.begin(), numbered_logs.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
        });

        for (const auto& log : numbered_logs) {
            int new_num = log.first + 1;
            std::string new_filename = base_filename_ + "." + std::to_string(new_num) + file_ext;
            fs::rename(log.second, new_filename);
        }
    }

    void compress_and_rename(const std::string& input_filename, const std::string& output_filename) {
        std::ifstream input_file(input_filename, std::ios::binary);
        if (!input_file) {
            std::cerr << "Failed to open input file: " << input_filename << std::endl;
            return;
        }

        gzFile output_file = gzopen(output_filename.c_str(), "wb");
        if (!output_file) {
            std::cerr << "Failed to open output file: " << output_filename << std::endl;
            input_file.close();
            return;
        }

        const size_t buffer_size = 1024;
        char buffer[buffer_size];
        while (input_file.read(buffer, buffer_size)) {
            gzwrite(output_file, buffer, buffer_size);
        }
        size_t remaining = input_file.gcount();
        if (remaining > 0) {
            gzwrite(output_file, buffer, remaining);
        }

        gzclose(output_file);
        input_file.close();
        fs::remove(input_filename);
    }

    void cleanup_old_logs() {
        std::vector<std::pair<int, std::string>> numbered_logs;
        std::string base_name = fs::path(base_filename_).filename().string();
        std::string file_ext = COMPRESS_LOG_FILES ? ".log.gz" : ".log";
        for (const auto& entry : fs::directory_iterator(fs::path(base_filename_).parent_path())) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                size_t pos1 = filename.find(base_name + ".");
                if (pos1 != std::string::npos) {
                    size_t pos2 = filename.find(file_ext, pos1 + base_name.length() + 1);
                    if (pos2 != std::string::npos) {
                        std::string num_str = filename.substr(pos1 + base_name.length() + 1, pos2 - (pos1 + base_name.length() + 1));
                        try {
                            int num = std::stoi(num_str);
                            numbered_logs.emplace_back(num, entry.path().string());
                        } catch (const std::invalid_argument&) {}
                    }
                }
            }
        }
        std::sort(numbered_logs.begin(), numbered_logs.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        if (numbered_logs.size() > MAX_ROLLED_LOG_FILES) {
            for (size_t i = MAX_ROLLED_LOG_FILES; i < numbered_logs.size(); ++i) {
                fs::remove(numbered_logs[i].second);
            }
        }
    }

    std::string get_current_time_with_milliseconds() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::tm* local_time = std::localtime(&now_c);
        char time_buffer[20];
        std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d:%H:%M:%S", local_time);
        std::stringstream ss;
        ss << time_buffer << ":" << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    std::string get_timestamp() {
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", local_time);
        return std::string(buffer);
    }

    template<typename T>
    void log(log_level level, const std::string& file, int line, const T& value) {
        // 根据 DEBUG 宏决定是否输出 DEBUG 级别日志
#ifdef DEBUG
        bool should_log = true;
#else
        bool should_log = level != log_level::DBG;
#endif

        if (should_log) {
            std::lock_guard<std::mutex> lock(mtx_);
            std::ostringstream oss;
            std::string level_str;
            switch (level) {
                case log_level::DBG:
                    level_str = "DBG";
                    break;
                case log_level::INFO:
                    level_str = "INFO";
                    break;
                case log_level::WARN:
                    level_str = "WARN";
                    break;
                case log_level::ERROR:
                    level_str = "ERROR";
                    break;
            }
            oss << "[" << get_current_time_with_milliseconds() << " " << fs::path(file).filename().string() << ":" << line << "] [" << level_str << "] " << value << std::endl;
            std::string log_msg = oss.str();

            if (!log_file_.is_open()) {
                open_log_file();
            }

            if (log_file_.is_open()) {
                log_file_ << log_msg;
                log_file_.flush();
                current_size_ += log_msg.size();

                if (current_size_ >= max_size_) {
                    roll_over();
                }
            }

            // 同时输出到控制台
            std::cout << log_msg;
        }
    }

    std::string base_filename_;
    size_t max_size_;
    size_t current_size_ = 0;
    std::ofstream log_file_;
    std::mutex mtx_;
};

// 方便使用的宏定义
#define LOG_DBG(...) logger::get_instance().dbg(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_INFO(...) logger::get_instance().info(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_WARN(...) logger::get_instance().warn(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_ERROR(...) logger::get_instance().error(__FILE__, __LINE__) << __VA_ARGS__
#define LOG_DBG_F(fmt, ...) logger::get_instance().logf(log_level::DBG, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_INFO_F(fmt, ...) logger::get_instance().logf(log_level::INFO, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_WARN_F(fmt, ...) logger::get_instance().logf(log_level::WARN, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_ERROR_F(fmt, ...) logger::get_instance().logf(log_level::ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__)
