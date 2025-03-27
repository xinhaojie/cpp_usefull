/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 13:02:40
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-19 13:06:52
* @FilePath     : /cpp_usefull/string/tiny_str.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_str.hpp"

namespace tiny_str {

// str::cat
template<typename ...X>
inline std::string cat(X&& ...x) {
    std::ostringstream oss;
    (oss << ... << std::forward<X>(x));
    return oss.str();
}

// str::replace
std::string replace(const char* s, size_t n, const char* sub, size_t m, const char* to, size_t l, size_t t = 0) {
    std::string result(s, n);
    size_t pos = 0;
    size_t count = 0;
    while ((t == 0 || count < t) && (pos = result.find(sub, pos, m)) != std::string::npos) {
        result.replace(pos, m, to, l);
        pos += l;
        ++count;
    }
    return result;
}

std::string replace(const char* s, size_t n, const char* sub, const char* to, size_t t = 0) {
    return replace(s, n, sub, std::strlen(sub), to, std::strlen(to), t);
}

std::string replace(const char* s, const char* sub, const char* to, size_t t = 0) {
    return replace(s, std::strlen(s), sub, std::strlen(sub), to, std::strlen(to), t);
}

std::string replace(const std::string& s, const char* sub, const char* to, size_t t = 0) {
    return replace(s.c_str(), s.size(), sub, std::strlen(sub), to, std::strlen(to), t);
}

// str::split
std::vector<std::string> split(const char* s, size_t n, char c, size_t t = 0) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t count = 0;
    for (size_t i = 0; i < n; ++i) {
        if ((t == 0 || count < t - 1) && s[i] == c) {
            result.emplace_back(s + start, i - start);
            start = i + 1;
            ++count;
        }
    }
    result.emplace_back(s + start, n - start);
    return result;
}

std::vector<std::string> split(const char* s, size_t n, const char* c, size_t m, size_t t = 0) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t count = 0;
    for (size_t i = 0; i <= n - m; ++i) {
        if ((t == 0 || count < t - 1) && std::strncmp(s + i, c, m) == 0) {
            result.emplace_back(s + start, i - start);
            start = i + m;
            ++count;
            i += m - 1;
        }
    }
    result.emplace_back(s + start, n - start);
    return result;
}

std::vector<std::string> split(const char* s, char c, size_t t = 0) {
    return split(s, std::strlen(s), c, t);
}

std::vector<std::string> split(const std::string& s, char c, size_t t = 0) {
    return split(s.c_str(), s.size(), c, t);
}

std::vector<std::string> split(const char* s, const char* c, size_t t = 0) {
    return split(s, std::strlen(s), c, std::strlen(c), t);
}

std::vector<std::string> split(const std::string& s, const char* c, size_t t = 0) {
    return split(s.c_str(), s.size(), c, std::strlen(c), t);
}

// str::strip
template<typename ...X>
inline std::string strip(X&& ...x) {
    return trim(std::forward<X>(x)...);
}

// str::trim
std::string trim(const char* s, const char* c = " \t\r\n", char d = 'b') {
    std::string str(s);
    size_t start = 0;
    size_t end = str.size();
    if (d == 'l' || d == 'L') {
        while (start < end && std::strchr(c, str[start]) != nullptr) {
            ++start;
        }
    } else if (d == 'r' || d == 'R') {
        while (end > start && std::strchr(c, str[end - 1]) != nullptr) {
            --end;
        }
    } else {
        while (start < end && std::strchr(c, str[start]) != nullptr) {
            ++start;
        }
        while (end > start && std::strchr(c, str[end - 1]) != nullptr) {
            --end;
        }
    }
    return str.substr(start, end - start);
}

std::string trim(const char* s, char c, char d = 'b') {
    char buf[2] = {c, '\0'};
    return trim(s, buf, d);
}

std::string trim(const std::string& s, const char* c = " \t\r\n", char d = 'b') {
    return trim(s.c_str(), c, d);
}

std::string trim(const std::string& s, char c, char d = 'b') {
    char buf[2] = {c, '\0'};
    return trim(s.c_str(), buf, d);
}

// str::to_bool
bool to_bool(const char* s) {
    if (std::strcmp(s, "0") == 0 || std::strcmp(s, "false") == 0) {
        errno = 0;
        return false;
    } else if (std::strcmp(s, "1") == 0 || std::strcmp(s, "true") == 0) {
        errno = 0;
        return true;
    }
    errno = EINVAL;
    return false;
}

bool to_bool(const std::string& s) {
    return to_bool(s.c_str());
}

// str::to_double
double to_double(const char* s) {
    char* endptr;
    double result = std::strtod(s, &endptr);
    if (*endptr != '\0' || (result == 0 && errno != 0)) {
        errno = (result == 0) ? EINVAL : ERANGE;
        return 0;
    }
    errno = 0;
    return result;
}

double to_double(const std::string& s) {
    return to_double(s.c_str());
}

// str::to_int
template<typename T>
T to_int(const char* s) {
    char* endptr;
    T result = 0;
    if constexpr (std::is_signed_v<T>) {
        result = static_cast<T>(std::strtoll(s, &endptr, 10));
    } else {
        result = static_cast<T>(std::strtoull(s, &endptr, 10));
    }
    if (*endptr != '\0') {
        if (*endptr == 'k' || *endptr == 'K') {
            result *= 1024;
            ++endptr;
        } else if (*endptr == 'm' || *endptr == 'M') {
            result *= 1024 * 1024;
            ++endptr;
        } else if (*endptr == 'g' || *endptr == 'G') {
            result *= 1024 * 1024 * 1024;
            ++endptr;
        } else if (*endptr == 't' || *endptr == 'T') {
            result *= static_cast<T>(1024ULL * 1024 * 1024 * 1024);
            ++endptr;
        } else if (*endptr == 'p' || *endptr == 'P') {
            result *= static_cast<T>(1024ULL * 1024 * 1024 * 1024 * 1024);
            ++endptr;
        }
    }
    if (*endptr != '\0' || (result == 0 && errno != 0)) {
        errno = (result == 0) ? EINVAL : ERANGE;
        return 0;
    }
    errno = 0;
    return result;
}

int32 to_int32(const char* s) {
    return to_int<int32>(s);
}

int32 to_int32(const std::string& s) {
    return to_int32(s.c_str());
}

int64 to_int64(const char* s) {
    return to_int<int64>(s);
}

int64 to_int64(const std::string& s) {
    return to_int64(s.c_str());
}

uint32 to_uint32(const char* s) {
    return to_int<uint32>(s);
}

uint32 to_uint32(const std::string& s) {
    return to_uint32(s.c_str());
}

uint64 to_uint64(const char* s) {
    return to_int<uint64>(s);
}

uint64 to_uint64(const std::string& s) {
    return to_uint64(s.c_str());
}

// str::from
template<typename T>
inline std::string from(T t) {
    return std::to_string(t);
}

template<>
inline std::string from<bool>(bool t) {
    return t ? "true" : "false";
}

// str::dbg
template<typename T>
std::string dbg(const std::vector<T>& v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) oss << ",";
        oss << dbg(v[i]);
    }
    oss << "]";
    return oss.str();
}

template<typename T>
std::string dbg(const std::set<T>& v) {
    std::ostringstream oss;
    oss << "{";
    auto it = v.begin();
    if (it != v.end()) {
        oss << dbg(*it);
        ++it;
    }
    for (; it != v.end(); ++it) {
        oss << "," << dbg(*it);
    }
    oss << "}";
    return oss.str();
}

template<typename K, typename V>
std::string dbg(const std::map<K, V>& v) {
    std::ostringstream oss;
    oss << "{";
    auto it = v.begin();
    if (it != v.end()) {
        oss << dbg(it->first) << ":" << dbg(it->second);
        ++it;
    }
    for (; it != v.end(); ++it) {
        oss << "," << dbg(it->first) << ":" << dbg(it->second);
    }
    oss << "}";
    return oss.str();
}

// 基础类型的 dbg
template<typename T>
std::enable_if_t<std::is_fundamental_v<T>, std::string> dbg(const T& t) {
    return std::to_string(t);
}

template<>
std::string dbg(const std::string& s) {
    return s;
}

} // namespace tiny_str
