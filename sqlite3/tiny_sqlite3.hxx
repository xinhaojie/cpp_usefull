#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

namespace tiny_sqlite3 {
// 自定义的 SQLite 异常类
class SQLiteException : public std::exception {
public:
    SQLiteException(const std::string& message);
    const char* what() const noexcept override;
private:
    std::string msg;
};

// SQLite3 封装类
class SQLiteWrapper {
public:
    // 默认使用 INCREMENTAL 模式
    SQLiteWrapper(const std::string& dbName, int autoVacuumMode = 2);
    ~SQLiteWrapper();

    // 执行 SQL 语句
    void execute(const std::string& sql);

    // 创建表
    void createTable(const std::string& tableName, const std::string& columnsDefinition);

    // 删除表
    void dropTable(const std::string& tableName);

    // 插入数据
    void insert(const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& values);

    // 删除数据
    void remove(const std::string& table, const std::string& condition);

    // 批量删除数据
    void batchRemove(const std::string& table, const std::vector<std::string>& conditions);

    // 修改数据
    void update(const std::string& table, const std::string& setClause, const std::string& condition);

    // 批量修改数据
    void batchUpdate(const std::string& table, const std::vector<std::pair<std::string, std::string>>& updates);

    // 查询数据，支持排序
    std::vector<std::vector<std::string>> select(const std::string& table, const std::vector<std::string>& columns,
                                                const std::string& condition = "", const std::string& orderBy = "");

    // 批量查询数据
    std::vector<std::vector<std::vector<std::string>>> batchSelect(const std::string& table, const std::vector<std::string>& columns,
                                                                    const std::vector<std::string>& conditions,
                                                                    const std::string& orderBy = "");
private:
    sqlite3* db;
};

}
