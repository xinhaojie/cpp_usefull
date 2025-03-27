#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace tiny_ini {


class IniParser {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    // 去除字符串首尾的空白字符
    std::string trim(const std::string& str);

public:
    // 从文件中读取 INI 数据
    bool load(const std::string& filename);

    // 保存 INI 数据到文件
    bool save(const std::string& filename);

    // 获取指定节和键的值
    std::string getValue(const std::string& section, const std::string& key);

    // 设置指定节和键的值
    void setValue(const std::string& section, const std::string& key, const std::string& value);

    // 删除指定节
    void deleteSection(const std::string& section);

    // 删除指定节中的键
    void deleteKey(const std::string& section, const std::string& key);

    // 获取所有节的名称
    std::vector<std::string> getSections();

    // 获取指定节中的所有键
    std::vector<std::string> getKeys(const std::string& section);
};

}
