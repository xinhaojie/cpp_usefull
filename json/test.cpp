#include <iostream>
#include <string>
#include "json.hxx"
int main() {
    try {
        // 创建 PeopleGroup 对象
        PeopleGroup group;
        group.group_name = "Team A";
        group.people = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}};

        // 设置 TypedValue
        TypedValue typedValue;
        typedValue.type = "object";
        Person personValue{"Charlie", 35};
        typedValue.value = personValue;

        group.typed_value = typedValue;

        // 序列化 PeopleGroup 对象到文件
        std::string filename = "people_group.json";
        if (MyJsonHandler::serialize_object_to_file(group, filename)) {
            std::cout << "成功将对象序列化到文件: " << filename << std::endl;
        }

        // 从文件中反序列化 PeopleGroup 对象
        PeopleGroup deserializedGroup = MyJsonHandler::deserialize_object_from_file<PeopleGroup>(filename);
        std::cout << "反序列化后的组名: " << deserializedGroup.group_name << std::endl;
        std::cout << "反序列化后的人员信息:" << std::endl;
        for (const auto& person : deserializedGroup.people) {
            std::cout << "  姓名: " << person.name << ", 年龄: " << person.age << std::endl;
        }

        // 根据 type 处理 value
        if (deserializedGroup.typed_value.type == "string") {
            std::string strValue = std::get<std::string>(deserializedGroup.typed_value.value);
            std::cout << "TypedValue 类型为 string，值为: " << strValue << std::endl;
        } else if (deserializedGroup.typed_value.type == "number") {
            int numValue = std::get<int>(deserializedGroup.typed_value.value);
            std::cout << "TypedValue 类型为 number，值为: " << numValue << std::endl;
        } else if (deserializedGroup.typed_value.type == "object") {
            Person objValue = std::get<Person>(deserializedGroup.typed_value.value);
            std::cout << "TypedValue 类型为 object，姓名: " << objValue.name << ", 年龄: " << objValue.age << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "主函数中出现未捕获的异常: " << e.what() << std::endl;
    }

    return 0;
}