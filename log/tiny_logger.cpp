#include "tiny_logger.hxx"
#include <iomanip>

logger& logger::get_instance(std::string log_path,
                            size_t max_size,
                            uint32_t file_counts,
                            bool compress,
                            bool output_to_file,
                            bool output_to_stdout,
                            log_level log_level){
    static logger instance(log_path, max_size, file_counts, compress, output_to_file, output_to_stdout, log_level);
    return instance;
}

logger::log_stream::log_stream(logger& logger, log_level level, const std::string& file, int line)
    : logger_(logger), level_(level), file_(file), line_(line) {}

logger::log_stream::~log_stream() {
    logger_.log(level_, file_, line_, oss_.str());
}

logger::log_stream logger::debug(const std::string& file, int line) {
    return log_stream(*this, log_level::_DEBUG, file, line);
}

logger::log_stream logger::info(const std::string& file, int line) {
    return log_stream(*this, log_level::_INFO, file, line);
}

logger::log_stream logger::warn(const std::string& file, int line) {
    return log_stream(*this, log_level::_WARN, file, line);
}

logger::log_stream logger::error(const std::string& file, int line) {
    return log_stream(*this, log_level::_ERROR, file, line);
}

void logger::logf(log_level level, const std::string& file, int line, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    log(level, file, line, buffer);
}

logger::logger(const std::string& base_filename, size_t max_size, uint32_t file_counts,
            bool compress, bool output_to_file, bool output_to_stdout, log_level log_level)
    : base_filename_(base_filename), max_size_(max_size), file_counts_(file_counts), compress_(compress),
        output_to_file_(output_to_file), output_to_stdout_(output_to_stdout), log_level_(log_level) {
    if (output_to_file_){
        create_log_directory();
        open_log_file();
    }
}

logger::~logger() {
    if (output_to_file_ && log_file_.is_open()){
        log_file_.close();
    }
}

void logger::create_log_directory() {
    fs::path log_dir = fs::path(base_filename_).parent_path();
    if (!fs::exists(log_dir)) {
        fs::create_directories(log_dir);
    }
}

void logger::open_log_file() {
    log_file_.open(base_filename_, std::ios::app);
    if (log_file_.is_open()) {
        current_size_ = fs::file_size(base_filename_);
    } else {
        std::cerr << "Failed to open log file: " << base_filename_ << std::endl;
    }
}

void logger::roll_over() {
    if (log_file_.is_open()) {
        log_file_.close();
    }

    // 重命名所有已存在的压缩日志文件，序号加 1
    rename_rolled_logs();

    std::string file_ext = compress_ ? ".log.gz" : ".log";
    std::string rolled_filename = base_filename_ + ".0" + file_ext;
    if (compress_){
    /* 压缩日志 xinhaojie 25-03-13 11:59:03*/
        compress_and_rename(base_filename_, rolled_filename);
    } else {
        fs::rename(base_filename_, rolled_filename);
    }

    // 重新打开新的日志文件
    open_log_file();

    // 清理多余的日志文件
    cleanup_old_logs();
}

void logger::rename_rolled_logs() {
    std::vector<std::pair<int, std::string>> numbered_logs;
    std::string base_name = fs::path(base_filename_).filename().string();
    std::string file_ext = compress_ ? ".log.gz" : ".log";
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

void logger::compress_and_rename(const std::string& input_filename, const std::string& output_filename) {
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

void logger::cleanup_old_logs() {
    std::vector<std::pair<int, std::string>> numbered_logs;
    std::string base_name = fs::path(base_filename_).filename().string();
    std::string file_ext = compress_ ? ".log.gz" : ".log";
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
    if (numbered_logs.size() > file_counts_) {
        for (size_t i = file_counts_; i < numbered_logs.size(); ++i) {
            fs::remove(numbered_logs[i].second);
        }
    }
}

std::string logger::get_current_time_with_milliseconds() {
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

std::string logger::get_timestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", local_time);
    return std::string(buffer);
}

template<typename T>
void logger::log(log_level level, const std::string& file, int line, const T& value) {
    // 根据 DEBUG 宏决定是否输出 DEBUG 级别日志
    if (level >= log_level_) {
        std::lock_guard<std::mutex> lock(mtx_);
        std::ostringstream oss;
        std::string level_str;
        switch (level) {
            case log_level::_DEBUG:
                level_str = "DEBUG";
                break;
            case log_level::_INFO:
                level_str = "INFO";
                break;
            case log_level::_WARN:
                level_str = "WARN";
                break;
            case log_level::_ERROR:
                level_str = "ERROR";
                break;
        }
        oss << "[" << get_current_time_with_milliseconds() << " " << fs::path(file).filename().string() << ":" << line << "] [" << level_str << "] " << value << std::endl;
        std::string log_msg = oss.str();
        if (output_to_file_) {
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
        }
        if (output_to_stdout_) {
            // 同时输出到控制台
            std::cout << log_msg;
        }
    }
}
