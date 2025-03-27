/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 16:27:22
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-27 14:01:07
* @FilePath     : /cpp_usefull/flag/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_flag.hpp"
#include <iostream>
#include <cassert>

using tiny_cmd_flag::CommandLineParser;
int main(int argc, char* argv[]) {
    // 创建解析器实例
    CommandLineParser parser("test_program");

    // 添加参数选项
    parser.add_option("i", "input", "", "Input file name", "", true);
    parser.add_option("o", "output", "default_output.txt", "Output file name");
    parser.add_option("m", "mode", "normal", "Running mode", "normal,fast,slow");
    parser.add_option("v", "verbose", "", "Enable verbose output");

    // 模拟命令行参数 --help
    const char* args_help[] = {
        "test_program",
        "--help"
    };
    int argc_help = sizeof(args_help) / sizeof(args_help[0]);
    bool result_help = parser.parse(argc_help, const_cast<char**>(args_help));
    assert(result_help == false);

    // 模拟命令行参数 -v
    const char* args_verbose[] = {
        "test_program",
        "-i", "test_input.txt",
        "-o", "test_output.txt",
        "-m", "fast",
        "-v"
    };
    int argc_verbose = sizeof(args_verbose) / sizeof(args_verbose[0]);
    bool result_verbose = parser.parse(argc_verbose, const_cast<char**>(args_verbose));
    assert(result_verbose == true);
    auto verbose = parser.get_option("verbose");
    assert(verbose.has_value() && verbose.value() == "true");

    std::cout << "All tests passed!" << std::endl;

    return 0;
}