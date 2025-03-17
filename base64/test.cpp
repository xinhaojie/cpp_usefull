/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:43:44
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-13 11:20:02
* @FilePath     : /cpp_usefull/base64/test.cpp
* @Description  : base64 测试
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
// main.cpp
#include "tiny_base64.hxx"
#include <iostream>

using tiny_base64::Base64;

int main() {
    std::string original_data = "Hello, Base64!";
    // 编码
    std::string encoded = Base64::encode(reinterpret_cast<const unsigned char*>(original_data.c_str()), original_data.length());
    std::cout << "Encoded: " << encoded << std::endl;

    // 解码
    std::vector<unsigned char> decoded = Base64::decode(encoded);
    std::string decoded_string(decoded.begin(), decoded.end());
    std::cout << "Decoded: " << decoded_string << std::endl;

    return 0;
}