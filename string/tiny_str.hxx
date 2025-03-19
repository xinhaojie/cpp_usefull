#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <cstring>
#include <cstdlib>
#include <cerrno>

namespace tiny_str {

// str::cat
template<typename ...X>
inline std::string cat(X&& ...x);

// str::replace
std::string replace(const char* s, size_t n, const char* sub, size_t m, const char* to, size_t l, size_t t = 0);
std::string replace(const char* s, size_t n, const char* sub, const char* to, size_t t = 0);
std::string replace(const char* s, const char* sub, const char* to, size_t t = 0);
std::string replace(const std::string& s, const char* sub, const char* to, size_t t = 0);

// str::split
std::vector<std::string> split(const char* s, size_t n, char c, size_t t = 0);
std::vector<std::string> split(const char* s, size_t n, const char* c, size_t m, size_t t = 0);
std::vector<std::string> split(const char* s, char c, size_t t = 0);
std::vector<std::string> split(const std::string& s, char c, size_t t = 0);
std::vector<std::string> split(const char* s, const char* c, size_t t = 0);
std::vector<std::string> split(const std::string& s, const char* c, size_t t = 0);

// str::strip
template<typename ...X>
inline std::string strip(X&& ...x);

// str::trim
std::string trim(const char* s, const char* c = " \t\r\n", char d = 'b');
std::string trim(const char* s, char c, char d = 'b');
std::string trim(const std::string& s, const char* c = " \t\r\n", char d = 'b');
std::string trim(const std::string& s, char c, char d = 'b');

// str::to_bool
bool to_bool(const char* s);
bool to_bool(const std::string& s);

// str::to_double
double to_double(const char* s);
double to_double(const std::string& s);

// str::to_int
using int32 = int32_t;
using int64 = int64_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

template<typename T>
T to_int(const char* s);

int32 to_int32(const char* s);
int32 to_int32(const std::string& s);
int64 to_int64(const char* s);
int64 to_int64(const std::string& s);
uint32 to_uint32(const char* s);
uint32 to_uint32(const std::string& s);
uint64 to_uint64(const char* s);
uint64 to_uint64(const std::string& s);

// str::from
template<typename T>
inline std::string from(T t);

template<>
inline std::string from<bool>(bool t);

// str::dbg
template<typename T>
std::string dbg(const std::vector<T>& v);

template<typename T>
std::string dbg(const std::set<T>& v);

template<typename K, typename V>
std::string dbg(const std::map<K, V>& v);

// 基础类型的 dbg
template<typename T>
std::enable_if_t<std::is_fundamental_v<T>, std::string> dbg(const T& t);

template<>
std::string dbg(const std::string& s);

} // namespace tiny_str
    