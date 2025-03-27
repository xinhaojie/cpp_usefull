/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 09:41:54
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-19 09:47:34
* @FilePath     : /cpp_usefull/path/tiny_path.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_path.hpp"
#include <sstream>
#include <algorithm>

namespace tiny_path {
// path::clean
std::string path::clean(const char* s, size_t n) {
    return clean(std::string(s, n));
}

std::string path::clean(const char* s) {
    return clean(std::string(s));
}

std::string path::clean(const std::string& s) {
    if (s.empty()) return ".";
    std::vector<std::string> parts;
    std::istringstream iss(s);
    std::string part;
    while (std::getline(iss, part, '/')) {
        if (part.empty() || part == ".") continue;
        if (part == "..") {
            if (!parts.empty() && parts.back() != "..") {
                parts.pop_back();
            } else {
                parts.push_back("..");
            }
        } else {
            parts.push_back(part);
        }
    }
    std::string result;
    for (const auto& p : parts) {
        if (!result.empty()) result += "/";
        result += p;
    }
    if (result.empty()) {
        if (s[0] == '/') return "/";
        return ".";
    }
    if (s[0] == '/') result = "/" + result;
    return result;
}

// path::split
std::pair<std::string, std::string> path::split(const char* s, size_t n) {
    return split(std::string(s, n));
}

std::pair<std::string, std::string> path::split(const char* s) {
    return split(std::string(s));
}

std::pair<std::string, std::string> path::split(const std::string& s) {
    size_t pos = s.rfind('/');
    if (pos == std::string::npos) return {"", s};
    return {s.substr(0, pos + 1), s.substr(pos + 1)};
}

// path::dir
std::string path::dir(const char* s, size_t n) {
    return dir(std::string(s, n));
}

std::string path::dir(const char* s) {
    return dir(std::string(s));
}

std::string path::dir(const std::string& s) {
    return clean(split(s).first);
}

// path::base
std::string path::base(const char* s, size_t n) {
    return base(std::string(s, n));
}

std::string path::base(const char* s) {
    return base(std::string(s));
}

std::string path::base(const std::string& s) {
    if (s.empty()) return ".";
    if (std::all_of(s.begin(), s.end(), [](char c) { return c == '/'; })) return "/";
    std::string trimmed = s;
    while (!trimmed.empty() && trimmed.back() == '/') {
        trimmed.pop_back();
    }
    size_t pos = trimmed.rfind('/');
    if (pos == std::string::npos) return trimmed;
    return trimmed.substr(pos + 1);
}

// path::ext
std::string path::ext(const char* s, size_t n) {
    return ext(std::string(s, n));
}

std::string path::ext(const char* s) {
    return ext(std::string(s));
}

std::string path::ext(const std::string& s) {
    size_t pos = s.rfind('.');
    if (pos == std::string::npos || pos == 0 || s.back() == '/') return "";
    return s.substr(pos);
}

}