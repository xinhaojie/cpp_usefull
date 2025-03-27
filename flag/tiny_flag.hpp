#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <set>

namespace tiny_cmd_flag {
class CommandLineParser {
private:
    std::unordered_map<std::string, std::tuple<std::string, std::string, std::string>> options;
    std::unordered_map<std::string, std::string> parsed_values;
    std::unordered_map<std::string, std::string> short_to_long_option;
    std::string program_name;
    std::set<std::string> required_options;

public:
    CommandLineParser(const std::string& name);

    void add_option(const std::string& short_option, const std::string& long_option, const std::string& default_value, const std::string& description, const std::string& supported_values = "", bool is_required = false);

    bool parse(int argc, char* argv[]);

    std::optional<std::string> get_option(const std::string& option) const;

    void print_help() const;
};

}
