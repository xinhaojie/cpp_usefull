#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <nlohmann/json.hpp>

// 定义自己的 JSON 序列化及反序列化类
class MyJsonHandler {
public:
    // 序列化对象为 JSON 字符串
    template<typename T>
    static std::string serialize_object(const T& obj) {
        try {
            nlohmann::json j = obj;
            return j.dump();
        } catch (const std::exception& e) {
            std::cerr << "序列化对象时出现异常: " << e.what() << std::endl;
            return "";
        }
    }

    // 反序列化 JSON 字符串为对象
    template<typename T>
    static T deserialize_object(const std::string& jsonStr) {
        try {
            nlohmann::json j = nlohmann::json::parse(jsonStr);
            return j.get<T>();
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON 解析错误: " << e.what() << std::endl;
            return T();
        } catch (const std::exception& e) {
            std::cerr << "反序列化对象时出现异常: " << e.what() << std::endl;
            return T();
        }
    }

    // 序列化对象到文件
    template<typename T>
    static bool serialize_object_to_file(const T& obj, const std::string& filename) {
        try {
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cerr << "无法打开文件: " << filename << std::endl;
                return false;
            }
            std::string jsonStr = serialize_object(obj);
            file << jsonStr;
            file.close();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "序列化对象到文件时出现异常: " << e.what() << std::endl;
            return false;
        }
    }

    // 从文件中反序列化对象
    template<typename T>
    static T deserialize_object_from_file(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "无法打开文件: " << filename << std::endl;
                return T();
            }
            std::string jsonStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            return deserialize_object<T>(jsonStr);
        } catch (const std::exception& e) {
            std::cerr << "从文件中反序列化对象时出现异常: " << e.what() << std::endl;
            return T();
        }
    }
};

// 示例结构体
struct Person {
    std::string name;
    int age;
    // 使用 NLOHMANN_DEFINE_TYPE_INTRUSIVE 宏定义 to_json 和 from_json 函数
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Person, name, age)
};

// 示例包含 type 和 value 的类
struct TypedValue {
    std::string type;
    std::variant<std::string, int, Person> value;

    // 自定义 to_json 函数
    friend void to_json(nlohmann::json& j, const TypedValue& tv) {
        j["type"] = tv.type;
        if (tv.type == "string") {
            j["value"] = std::get<std::string>(tv.value);
        } else if (tv.type == "number") {
            j["value"] = std::get<int>(tv.value);
        } else if (tv.type == "object") {
            j["value"] = std::get<Person>(tv.value);
        }
    }

    // 自定义 from_json 函数
    friend void from_json(const nlohmann::json& j, TypedValue& tv) {
        j.at("type").get_to(tv.type);
        if (tv.type == "string") {
            tv.value = j.at("value").get<std::string>();
        } else if (tv.type == "number") {
            tv.value = j.at("value").get<int>();
        } else if (tv.type == "object") {
            tv.value = j.at("value").get<Person>();
        }
    }
};

// 示例包含结构体数组和 TypedValue 的类
struct PeopleGroup {
    std::string group_name;
    std::vector<Person> people;
    TypedValue typed_value;
    // 使用 NLOHMANN_DEFINE_TYPE_INTRUSIVE 宏定义 to_json 和 from_json 函数
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PeopleGroup, group_name, people, typed_value)
};