/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-18 16:07:47
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-18 17:19:54
* @FilePath     : /cpp_usefull/flag/tiny_flag.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_flag.hxx"
#include <sstream>

namespace tiny_cmd_flag {
CommandLineParser::CommandLineParser(const std::string& name) : program_name(name) {}

void CommandLineParser::add_option(const std::string& short_option, const std::string& long_option, const std::string& default_value, const std::string& description, const std::string& supported_values, bool is_required) {
    options[long_option] = std::make_tuple(default_value, description, supported_values);
    short_to_long_option[short_option] = long_option;
    if (is_required) {
        required_options.insert(long_option);
    }
}

bool CommandLineParser::parse(int argc, char* argv[]) {
    for (const auto& opt : options) {
        parsed_values[opt.first] = std::get<0>(opt.second);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        std::string option;
        std::string value;

        if (arg == "--help" || arg == "-h") {
            print_help();
            return false;
        }

        if (arg.substr(0, 2) == "--") {
            option = arg.substr(2);
        } else if (arg.substr(0, 1) == "-") {
            std::string short_opt = arg.substr(1);
            auto it = short_to_long_option.find(short_opt);
            if (it != short_to_long_option.end()) {
                option = it->second;
            }
        }

        if (i + 1 < argc && argv[i + 1][0] != '-') {
            value = argv[i + 1];
            ++i;
        } else if (option.find('=') != std::string::npos) {
            auto pos = option.find('=');
            value = option.substr(pos + 1);
            option = option.substr(0, pos);
        } else {
            value = "true";
        }

        if (options.find(option) != options.end()) {
            std::string supported_values = std::get<2>(options[option]);
            if (!supported_values.empty()) {
                std::istringstream iss(supported_values);
                std::string token;
                bool valid_value = false;
                while (std::getline(iss, token, ',')) {
                    if (token == value) {
                        valid_value = true;
                        break;
                    }
                }
                if (!valid_value) {
                    std::cerr << "Error: Invalid value '" << value << "' for option --" << option << ". Allowed values are: " << supported_values << std::endl;
                    print_help();
                    return false;
                }
            }
            parsed_values[option] = value;
        }
    }

    // 检查必需参数是否都已提供
    bool all_required_provided = true;
    for (const auto& opt : required_options) {
        if (parsed_values[opt] == std::get<0>(options[opt])) {
            std::cerr << "Error: Required option --" << opt << " is not provided." << std::endl;
            all_required_provided = false;
        }
    }

    if (!all_required_provided) {
        print_help();
        return false;
    }

    return true;
}

std::optional<std::string> CommandLineParser::get_option(const std::string& option) const {
    auto long_opt = short_to_long_option.find(option);
    std::string actual_option = long_opt != short_to_long_option.end() ? long_opt->second : option;
    auto it = parsed_values.find(actual_option);
    if (it != parsed_values.end()) {
        return it->second;
    }
    return std::nullopt;
}

void CommandLineParser::print_help() const {
    std::string usage = "Usage: " + program_name;
    std::string required_usage;
    std::string optional_usage;

    for (const auto& opt : options) {
        std::string long_option = opt.first;
        std::string default_value = std::get<0>(opt.second);
        std::string supported_values = std::get<2>(opt.second);
        std::string value_display;

        if (!supported_values.empty()) {
            std::stringstream ss(supported_values);
            std::string token;
            std::string values;
            while (std::getline(ss, token, ',')) {
                if (!values.empty()) {
                    values += "|";
                }
                values += token;
            }
            value_display = "{" + values + "}";
        } else {
            value_display = "<" + default_value + ">";
        }

        std::string short_option;
        for (const auto& pair : short_to_long_option) {
            if (pair.second == long_option) {
                short_option = pair.first;
                break;
            }
        }

        std::string option_display;
        if (!short_option.empty()) {
            option_display = "-" + short_option + ", --" + long_option;
        } else {
            option_display = "--" + long_option;
        }

        std::string equal_and_value = "";
        if (!default_value.empty() || !supported_values.empty()) {
            equal_and_value = "=" + value_display;
        }

        if (required_options.find(long_option) != required_options.end()) {
            required_usage += " " + option_display + equal_and_value;
        } else {
            optional_usage += " [" + option_display + equal_and_value + "]";
        }
    }

    usage += required_usage + optional_usage;
    std::cout << usage << std::endl;
    std::cout << "  --help, -h        Print this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Required Options:" << std::endl;
    for (const auto& opt : options) {
        std::string long_option = opt.first;
        if (required_options.find(long_option) != required_options.end()) {
            std::string supported = std::get<2>(opt.second);
            std::string display = supported.empty() ? "<" + std::get<0>(opt.second) + ">" : "{" + supported + "}";
            std::string short_option;
            for (const auto& pair : short_to_long_option) {
                if (pair.second == long_option) {
                    short_option = pair.first;
                    break;
                }
            }
            std::string option_display;
            if (!short_option.empty()) {
                option_display = "  -" + short_option + ", --" + long_option;
            } else {
                option_display = "  --" + long_option;
            }

            std::string equal_and_display = "";
            if (!std::get<0>(opt.second).empty() || !supported.empty()) {
                equal_and_display = "=" + display;
            }

            std::cout << option_display << equal_and_display;
            std::cout << " (required)";
            std::cout << "  " << std::get<1>(opt.second) << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "Optional Options:" << std::endl;
    for (const auto& opt : options) {
        std::string long_option = opt.first;
        if (required_options.find(long_option) == required_options.end()) {
            std::string supported = std::get<2>(opt.second);
            std::string display = supported.empty() ? "[" + std::get<0>(opt.second) + "]" : "{" + supported + "}";
            std::string short_option;
            for (const auto& pair : short_to_long_option) {
                if (pair.second == long_option) {
                    short_option = pair.first;
                    break;
                }
            }
            std::string option_display;
            if (!short_option.empty()) {
                option_display = "  -" + short_option + ", --" + long_option;
            } else {
                option_display = "  --" + long_option;
            }

            std::string equal_and_display = "";
            if (!std::get<0>(opt.second).empty() || !supported.empty()) {
                equal_and_display = "=" + display;
            }

            std::cout << option_display << equal_and_display;
            std::cout << "  " << std::get<1>(opt.second) << std::endl;
        }
    }
}

}