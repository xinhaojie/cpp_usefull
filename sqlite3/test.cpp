/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:35:39
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-12 16:35:44
* @FilePath     : /cpp_usefull/sqlite3/test.cpp
* @Description  : 
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "sqlite_wrapper.hpp"

int main() {
    try {
        // 这里默认使用 INCREMENTAL 模式，可不传第二个参数
        SQLiteWrapper db("test.db");

        // 创建表
        db.createTable("users", "id INTEGER PRIMARY KEY, name TEXT, age INTEGER");

        // 插入数据
        db.insert("users", {"name", "age"}, {"John", "25"});
        db.insert("users", {"name", "age"}, {"Jane", "30"});
        db.insert("users", {"name", "age"}, {"Doe", "22"});

        // 查询数据并按年龄升序排序
        auto result = db.select("users", {"name", "age"}, "", "age ASC");
        std::cout << "Query results sorted by age in ascending order:" << std::endl;
        for (const auto& row : result) {
            for (const auto& col : row) {
                std::cout << col << " ";
            }
            std::cout << std::endl;
        }

        // 多条件查询
        auto multiConditionResult = db.select("users", {"name", "age"}, "age > 20 AND age < 30", "name DESC");
        std::cout << "\nQuery results with multiple conditions sorted by name in descending order:" << std::endl;
        for (const auto& row : multiConditionResult) {
            for (const auto& col : row) {
                std::cout << col << " ";
            }
            std::cout << std::endl;
        }

        // 批量查询
        auto batchResults = db.batchSelect("users", {"name", "age"}, {"age > 20", "age < 30"}, "age ASC");
        std::cout << "\nBatch query results:" << std::endl;
        for (const auto& resultSet : batchResults) {
            for (const auto& row : resultSet) {
                for (const auto& col : row) {
                    std::cout << col << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        // 修改数据
        db.update("users", "age = 26", "name = 'John'");

        // 删除数据
        db.remove("users", "name = 'Doe'");

        // 批量删除
        db.batchRemove("users", {"age > 25"});

        // 删除表
        db.dropTable("users");

    } catch (const SQLiteException& e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }

    return 0;
}