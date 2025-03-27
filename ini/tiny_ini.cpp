/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:41:26
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-27 14:01:36
* @FilePath     : /cpp_usefull/ini/tiny_ini.cpp
* @Description  : ini文件解析
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_ini.hpp"

namespace tiny_ini {

// 去除字符串首尾的空白字符
std::string IniParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// 从文件中读取 INI 数据
bool IniParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::string currentSection;
    while (std::getline(file, line)) {
        // 移除行尾的注释（以 # 或 ; 开头）
        size_t commentPos1 = line.find('#');
        size_t commentPos2 = line.find(';');
        size_t commentPos = std::string::npos;
        if (commentPos1 != std::string::npos && commentPos2 != std::string::npos) {
            commentPos = std::min(commentPos1, commentPos2);
        } else if (commentPos1 != std::string::npos) {
            commentPos = commentPos1;
        } else if (commentPos2 != std::string::npos) {
            commentPos = commentPos2;
        }
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = trim(line);
        if (line.empty()) {
            continue;
        }
        if (line[0] == '[') {
            size_t end = line.find(']');
            if (end != std::string::npos) {
                currentSection = line.substr(1, end - 1);
                data[currentSection];
            }
        } else {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = trim(line.substr(0, pos));
                std::string value = trim(line.substr(pos + 1));
                if (!currentSection.empty()) {
                    data[currentSection][key] = value;
                }
            }
        }
    }
    file.close();
    return true;
}

// 保存 INI 数据到文件
bool IniParser::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& section : data) {
        file << "[" << section.first << "]" << std::endl;
        for (const auto& keyValue : section.second) {
            file << keyValue.first << " = " << keyValue.second << std::endl;
        }
        file << std::endl;
    }
    file.close();
    return true;
}

// 获取指定节和键的值
std::string IniParser::getValue(const std::string& section, const std::string& key) {
    auto secIt = data.find(section);
    if (secIt != data.end()) {
        auto keyIt = secIt->second.find(key);
        if (keyIt != secIt->second.end()) {
            return keyIt->second;
        }
    }
    return "";
}

// 设置指定节和键的值
void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
    data[section][key] = value;
}

// 删除指定节
void IniParser::deleteSection(const std::string& section) {
    data.erase(section);
}

// 删除指定节中的键
void IniParser::deleteKey(const std::string& section, const std::string& key) {
    auto secIt = data.find(section);
    if (secIt != data.end()) {
        secIt->second.erase(key);
    }
}

// 获取所有节的名称
std::vector<std::string> IniParser::getSections() {
    std::vector<std::string> sections;
    for (const auto& section : data) {
        sections.push_back(section.first);
    }
    return sections;
}

// 获取指定节中的所有键
std::vector<std::string> IniParser::getKeys(const std::string& section) {
    std::vector<std::string> keys;
    auto secIt = data.find(section);
    if (secIt != data.end()) {
        for (const auto& keyValue : secIt->second) {
            keys.push_back(keyValue.first);
        }
    }
    return keys;
}

}