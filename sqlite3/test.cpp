#include "tiny_sqlite3.hpp"

int main() {
    try {
        tiny_sqlite3::SQLiteWrapper db("test.db");
        db.set_auto_vacuum_mode(1);

        // 创建表
        db.create_table("test_table", "id INT, name TEXT, num REAL, date_time TEXT");

        // 开始事务
        db.begin_transaction();

        try {
            // 插入数据
            auto now = std::chrono::system_clock::now();
            std::vector<tiny_sqlite3::Value> values = {1, "John", 3.14, now};
            db.insert("test_table", "id, name, num, date_time", values);

            // 更新数据
            db.update("test_table", "name = 'Jane'", "id = 1");

            // 提交事务
            db.commit_transaction();
            std::cout << "Transaction committed successfully." << std::endl;
        } catch (const std::exception& e) {
            // 回滚事务
            db.rollback_transaction();
            std::cerr << "Transaction rolled back due to error: " << e.what() << std::endl;
        }

        // 查询数据
        tiny_sqlite3::TableWrapper result;
        db.select("*", "test_table", result);

        // 遍历查询结果
        result.for_each_row([](const std::unordered_map<std::string, tiny_sqlite3::Value>& row) {
            for (const auto& [column, value] : row) {
                if (std::holds_alternative<int>(value)) {
                    std::cout << column << ": " << std::get<int>(value) << "\t";
                } else if (std::holds_alternative<double>(value)) {
                    std::cout << column << ": " << std::get<double>(value) << "\t";
                } else if (std::holds_alternative<std::string>(value)) {
                    std::cout << column << ": " << std::get<std::string>(value) << "\t";
                } else if (std::holds_alternative<tiny_sqlite3::TimePoint>(value)) {
                    std::cout << column << ": " << result.time_point_to_string(std::get<tiny_sqlite3::TimePoint>(value)) << "\t";
                }
            }
            std::cout << std::endl;
        });

        // 根据行索引和列名获取数据
        try {
            const auto& value = result.get_value(0, "date_time");
            if (std::holds_alternative<tiny_sqlite3::TimePoint>(value)) {
                std::cout << "Value at row 0, column 'date_time': " << result.time_point_to_string(std::get<tiny_sqlite3::TimePoint>(value)) << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error getting value: " << e.what() << std::endl;
        }

        // 删除表
        db.drop_table("test_table");

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}