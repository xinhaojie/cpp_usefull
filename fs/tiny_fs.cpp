/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 09:56:23
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-19 10:02:02
* @FilePath     : /cpp_usefull/fs/tiny_fs.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_fs.hxx"
#include <cstdio>
#include <cstring>

namespace tiny_fs {
    file::file() : open_mode('\0') {}

    file::file(file&& f) noexcept : stream(std::move(f.stream)), file_path(std::move(f.file_path)), open_mode(f.open_mode) {
        f.open_mode = '\0';
    }

    file::file(const std::string& path, char mode) : open_mode(mode) {
        open(path, mode);
    }

    file::~file() {
        close();
    }

    void file::close() {
        if (stream.is_open()) {
            stream.close();
        }
    }

    bool file::exists() const {
        return std::filesystem::exists(file_path);
    }

    bool file::open(const std::string& path, char mode) {
        close();
        file_path = path;
        open_mode = mode;
        std::ios_base::openmode flags;
        switch (mode) {
            case 'r':
                flags = std::ios::in;
                break;
            case 'w':
                flags = std::ios::out | std::ios::trunc;
                break;
            case 'a':
                flags = std::ios::out | std::ios::app;
                break;
            case 'm':
                flags = std::ios::out | std::ios::in;
                break;
            case '+':
                flags = std::ios::in | std::ios::out;
                break;
            default:
                return false;
        }
        stream.open(file_path, flags);
        return stream.is_open();
    }

    file::operator bool() const {
        return stream.is_open();
    }

    bool file::operator!() const {
        return !stream.is_open();
    }

    const std::string& file::path() const {
        return file_path;
    }

    size_t file::read(void* buf, size_t n) {
        if (stream.is_open()) {
            stream.read(static_cast<char*>(buf), n);
            return stream.gcount();
        }
        return 0;
    }

    std::string file::read(size_t n) {
        std::string result(n, '\0');
        size_t read_size = read(&result[0], n);
        result.resize(read_size);
        return result;
    }

    void file::seek(int64_t off, int whence) {
        if (stream.is_open() && open_mode != 'a') {
            stream.seekg(off, static_cast<std::ios_base::seekdir>(whence));
            stream.seekp(off, static_cast<std::ios_base::seekdir>(whence));
        }
    }

    // 修改：去掉 const 限定符
    int64_t file::size() {
        if (stream.is_open()) {
            auto current = stream.tellg();
            stream.seekg(0, std::ios::end);
            auto size = stream.tellg();
            stream.seekg(current);
            return size;
        }
        return -1;
    }

    size_t file::write(const void* s, size_t n) {
        if (stream.is_open()) {
            stream.write(static_cast<const char*>(s), n);
            return stream.gcount();
        }
        return 0;
    }

    size_t file::write(const char* s) {
        return write(s, std::strlen(s));
    }

    size_t file::write(const std::string& s) {
        return write(s.data(), s.size());
    }

    size_t file::write(char c) {
        return write(&c, 1);
    }

    fstream::fstream() : buffer_capacity(8192), buffer(buffer_capacity) {}

    fstream::fstream(fstream&& fs) noexcept : stream(std::move(fs.stream)), file_path(std::move(fs.file_path)),
                                               open_mode(fs.open_mode), buffer_capacity(fs.buffer_capacity),
                                               buffer(std::move(fs.buffer)) {
        fs.open_mode = '\0';
    }

    fstream::fstream(size_t cap) : buffer_capacity(cap), buffer(buffer_capacity) {}

    fstream::fstream(const std::string& path, char mode, size_t cap) : buffer_capacity(cap), buffer(buffer_capacity) {
        open(path, mode);
    }

    fstream::~fstream() {
        close();
    }

    fstream& fstream::append(const void* s, size_t n) {
        if (stream.is_open()) {
            if (buffer.size() - buffer.size() < n) {
                flush();
            }
            std::memcpy(buffer.data() + buffer.size(), s, n);
            buffer.resize(buffer.size() + n);
        }
        return *this;
    }

    void fstream::close() {
        flush();
        if (stream.is_open()) {
            stream.close();
        }
    }

    void fstream::flush() {
        if (stream.is_open() &&!buffer.empty()) {
            stream.write(buffer.data(), buffer.size());
            buffer.clear();
        }
    }

    bool fstream::open(const std::string& path, char mode) {
        close();
        file_path = path;
        open_mode = mode;
        std::ios_base::openmode flags;
        switch (mode) {
            case 'w':
                flags = std::ios::out | std::ios::trunc;
                break;
            case 'a':
                flags = std::ios::out | std::ios::app;
                break;
            default:
                return false;
        }
        stream.open(file_path, flags);
        return stream.is_open();
    }

    fstream::operator bool() const {
        return stream.is_open();
    }

    bool fstream::operator!() const {
        return !stream.is_open();
    }

    fstream& fstream::operator<<(const char* s) {
        return append(s, std::strlen(s));
    }

    fstream& fstream::operator<<(const std::string& s) {
        return append(s.data(), s.size());
    }

    template<typename T>
    fstream& fstream::operator<<(T v) {
        std::string str = std::to_string(v);
        return append(str.data(), str.size());
    }

    void fstream::reserve(size_t n) {
        if (n > buffer_capacity) {
            buffer_capacity = n;
            buffer.reserve(buffer_capacity);
        }
    }

    dir::dir() {}

    dir::dir(dir&& d) noexcept : dir_iter(std::move(d.dir_iter)), dir_path(std::move(d.dir_path)) {}

    dir::dir(const std::string& path) {
        open(path);
    }

    dir::~dir() {
        close();
    }

    std::vector<std::string> dir::all() const {
        std::vector<std::string> result;
        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            if (entry.path().filename() != "." && entry.path().filename() != "..") {
                result.push_back(entry.path().filename().string());
            }
        }
        return result;
    }

    dir::iterator dir::begin() const {
        iterator it;
        it.iter = std::filesystem::directory_iterator(dir_path);
        return it;
    }

    dir::iterator dir::end() const {
        iterator it;
        it.iter = std::filesystem::directory_iterator();
        return it;
    }

    void dir::close() {
        dir_iter = std::filesystem::directory_iterator();
    }

    bool dir::open(const std::string& path) {
        close();
        dir_path = path;
        if (std::filesystem::is_directory(path)) {
            dir_iter = std::filesystem::directory_iterator(path);
            return true;
        }
        return false;
    }

    const std::string& dir::path() const {
        return dir_path;
    }

    std::string dir::iterator::operator*() const {
        return iter->path().filename().string();
    }

    dir::iterator& dir::iterator::operator++() {
        ++iter;
        return *this;
    }

    bool dir::iterator::operator==(const iterator& it) const {
        return iter == it.iter;
    }

    bool dir::iterator::operator!=(const iterator& it) const {
        return iter != it.iter;
    }
}