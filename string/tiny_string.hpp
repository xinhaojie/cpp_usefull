#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>

namespace tiny_string {
    class fastring {
    private:
        char* data_;
        size_t size_;
        size_t capacity_;
        static const size_t npos = (size_t)-1;

        void ensure_capacity(size_t new_size);

    public:
        // 默认构造函数
        constexpr fastring() noexcept;

        // 指定初始容量的构造函数
        explicit fastring(size_t cap);

        // 用给定的字节序列创建 fastring 对象
        fastring(const void* s, size_t n);

        // 用 C 风格的字符串创建 fastring 对象
        fastring(const char* s);

        // 用 std::string 创建一个 fastring 对象
        fastring(const std::string& s);

        // 将 fastring 对象初始化为 n 个字符 c 构成的字符串
        fastring(size_t n, char c);

        // 拷贝构造函数
        fastring(const fastring& s);

        // 移动构造函数
        fastring(fastring&& s) noexcept;

        // 析构函数
        ~fastring();

        // 赋值运算符重载
        fastring& operator=(const char* s);
        fastring& operator=(const std::string& s);
        fastring& operator=(const fastring& s);
        fastring& operator=(fastring&& s) noexcept;

        // assign 方法
        fastring& assign(const void* s, size_t n);
        template<typename S>
        fastring& assign(S&& s);
        fastring& assign(size_t n, char c);

        // back 方法
        char& back();
        const char& back() const;

        // front 方法
        char& front();
        const char& front() const;

        // 下标运算符重载
        char& operator[](size_t n);
        const char& operator[](size_t n) const;

        // capacity 方法
        size_t capacity() const noexcept;

        // c_str 方法
        const char* c_str() const;

        // data 方法
        char* data() noexcept;
        const char* data() const noexcept;

        // empty 方法
        bool empty() const noexcept;

        // size 方法
        size_t size() const noexcept;

        // clear 方法
        void clear();
        void clear(char c);

        // ensure 方法
        void ensure(size_t n);

        // reserve 方法
        void reserve(size_t n);

        // reset 方法
        void reset();

        // resize 方法
        void resize(size_t n);
        void resize(size_t n, char c);

        // shrink 方法
        void shrink();

        // swap 方法
        void swap(fastring& s) noexcept;
        void swap(fastring&& s) noexcept;

        // append 方法
        fastring& append(const void* s, size_t n);
        fastring& append(const char* s);
        fastring& append(const fastring& s);
        fastring& append(const std::string& s);
        fastring& append(size_t n, char c);
        fastring& append(char c);

        // append_nomchk 方法
        fastring& append_nomchk(const void* s, size_t n);
        fastring& append_nomchk(const char* s);

        // cat 方法
        template<typename X, typename ...V>
        fastring& cat(X&& x, V&& ... v);

        // 插入运算符重载
        fastring& operator<<(bool v);
        fastring& operator<<(char v);
        fastring& operator<<(signed char v);
        fastring& operator<<(unsigned char v);
        fastring& operator<<(short v);
        fastring& operator<<(unsigned short v);
        fastring& operator<<(int v);
        fastring& operator<<(unsigned int v);
        fastring& operator<<(long v);
        fastring& operator<<(unsigned long v);
        fastring& operator<<(long long v);
        fastring& operator<<(unsigned long long v);
        fastring& operator<<(double v);
        fastring& operator<<(float v);
        fastring& operator<<(const void* v);
        fastring& operator<<(std::nullptr_t);
        fastring& operator<<(const char* s);
        fastring& operator<<(const signed char* s);
        fastring& operator<<(const unsigned char* s);
        fastring& operator<<(const fastring& s);
        fastring& operator<<(const std::string& s);

        // += 运算符重载
        fastring& operator+=(const char* s);
        fastring& operator+=(const fastring& s);
        fastring& operator+=(const std::string& s);
        fastring& operator+=(char c);

        // push_back 方法
        fastring& push_back(char c);

        // pop_back 方法
        char pop_back();

        // compare 方法
        int compare(const char* s, size_t n) const;
        int compare(const char* s) const;
        int compare(const fastring& s) const noexcept;
        int compare(const std::string& s) const noexcept;
        int compare(size_t pos, size_t len, const char* s, size_t n) const;
        int compare(size_t pos, size_t len, const char* s) const;
        int compare(size_t pos, size_t len, const fastring& s) const;
        int compare(size_t pos, size_t len, const std::string& s) const;
        int compare(size_t pos, size_t len, const fastring& s, size_t spos, size_t n) const;
        int compare(size_t pos, size_t len, const std::string& s, size_t spos, size_t n) const;
    };
}