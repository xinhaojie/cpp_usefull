/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:41:41
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-12 16:41:45
* @FilePath     : /cpp_usefull/ini/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_ini.hxx"
#include <iostream>

using tiny_ini::IniParser;

int main() {
    IniParser parser;
    if (parser.load("example.ini")) {
        std::cout << "Loaded INI file successfully." << std::endl;
        std::string value = parser.getValue("Section1", "Key1");
        std::cout << "Value of Key1 in Section1: " << value << std::endl;

        parser.setValue("Section1", "Key1", "NewValue");
        if (parser.save("example.ini")) {
            std::cout << "Saved INI file successfully." << std::endl;
        } else {
            std::cout << "Failed to save INI file." << std::endl;
        }
    } else {
        std::cout << "Failed to load INI file." << std::endl;
    }

    return 0;
}