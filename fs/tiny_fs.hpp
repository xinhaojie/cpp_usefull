#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>

namespace tiny_fs {
    // 辅助函数，用于简化代码
    inline bool exists(const std::string& path) {
        return std::filesystem::exists(path);
    }

    inline int64_t fsize(const std::string& path) {
        if (exists(path)) {
            return std::filesystem::file_size(path);
        }
        return -1;
    }

    inline bool isdir(const std::string& path) {
        return std::filesystem::is_directory(path);
    }

    inline int64_t mtime(const std::string& path) {
        if (exists(path)) {
            auto time = std::filesystem::last_write_time(path);
            return std::chrono::duration_cast<std::chrono::seconds>(
                time.time_since_epoch()).count();
        }
        return -1;
    }

    inline bool mkdir(const std::string& path, bool p = false) {
        if (p) {
            return std::filesystem::create_directories(path);
        }
        return std::filesystem::create_directory(path);
    }

    inline bool mv(const std::string& from, const std::string& to) {
        try {
            std::filesystem::rename(from, to);
            return true;
        } catch (...) {
            return false;
        }
    }

    inline bool remove(const std::string& path, bool r = false) {
        if (r) {
            return std::filesystem::remove_all(path) > 0;
        }
        return std::filesystem::remove(path);
    }

    inline bool symlink(const std::string& dst, const std::string& lnk) {
        try {
            std::filesystem::remove(lnk);
            std::filesystem::create_symlink(dst, lnk);
            return true;
        } catch (...) {
            return false;
        }
    }

    class file {
    public:
        file();
        file(file&& f) noexcept;
        file(const std::string& path, char mode);
        ~file();

        void close();
        bool exists() const;
        bool open(const std::string& path, char mode);
        explicit operator bool() const;
        bool operator!() const;
        const std::string& path() const;
        size_t read(void* buf, size_t n);
        std::string read(size_t n);
        void seek(int64_t off, int whence = SEEK_SET);
        // 修改：去掉 const 限定符
        int64_t size(); 
        size_t write(const void* s, size_t n);
        size_t write(const char* s);
        size_t write(const std::string& s);
        size_t write(char c);

    private:
        std::fstream stream;
        std::string file_path;
        char open_mode;
    };

    class fstream {
    public:
        fstream();
        fstream(fstream&& fs) noexcept;
        explicit fstream(size_t cap);
        fstream(const std::string& path, char mode, size_t cap = 8192);
        ~fstream();

        fstream& append(const void* s, size_t n);
        void close();
        void flush();
        bool open(const std::string& path, char mode);
        explicit operator bool() const;
        bool operator!() const;
        fstream& operator<<(const char* s);
        fstream& operator<<(const std::string& s);
        template<typename T>
        fstream& operator<<(T v);
        void reserve(size_t n);

    private:
        std::ofstream stream;
        std::string file_path;
        char open_mode;
        size_t buffer_capacity;
        std::vector<char> buffer;
    };

    class dir {
    public:
        class iterator {
        public:
            std::string operator*() const;
            iterator& operator++();
            bool operator==(const iterator& it) const;
            bool operator!=(const iterator& it) const;

        private:
            std::filesystem::directory_iterator iter;
            friend class dir;
        };

        dir();
        dir(dir&& d) noexcept;
        explicit dir(const std::string& path);
        ~dir();

        std::vector<std::string> all() const;
        iterator begin() const;
        iterator end() const;
        void close();
        bool open(const std::string& path);
        const std::string& path() const;

    private:
        std::filesystem::directory_iterator dir_iter;
        std::string dir_path;
    };
}