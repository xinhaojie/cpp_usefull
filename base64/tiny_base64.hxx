#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace tiny_base64 {

class Base64 {
private:
    static const std::string base64_chars;

    static inline bool is_base64(unsigned char c);

public:
    // 将内存数据编码为Base64字符串
    static std::string encode(const unsigned char* bytes_to_encode, unsigned int in_len);

    // 将Base64字符串解码为内存数据
    static std::vector<unsigned char> decode(const std::string& encoded_string);
};

}
