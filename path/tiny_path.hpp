#pragma once

#include <string>
#include <utility>
#include <vector>

namespace tiny_path{
class path {
public:
    // path::clean
    static std::string clean(const char* s, size_t n);
    static std::string clean(const char* s);
    static std::string clean(const std::string& s);

    // path::join
    template<typename ...S>
    static inline std::string join(S&&... s) {
        std::string result;
        (void)std::initializer_list<int>{(
            [&result](const auto& str) {
                std::string strAsStdString = str; // 将传入的参数转换为 std::string
                if (!strAsStdString.empty()) {
                    if (!result.empty() && result.back() != '/') result += '/';
                    result += strAsStdString;
                }
            }(std::forward<S>(s)), 0)...};
        return clean(result);
    }

    // path::split
    static std::pair<std::string, std::string> split(const char* s, size_t n);
    static std::pair<std::string, std::string> split(const char* s);
    static std::pair<std::string, std::string> split(const std::string& s);

    // path::dir
    static std::string dir(const char* s, size_t n);
    static std::string dir(const char* s);
    static std::string dir(const std::string& s);

    // path::base
    static std::string base(const char* s, size_t n);
    static std::string base(const char* s);
    static std::string base(const std::string& s);

    // path::ext
    static std::string ext(const char* s, size_t n);
    static std::string ext(const char* s);
    static std::string ext(const std::string& s);
};

}