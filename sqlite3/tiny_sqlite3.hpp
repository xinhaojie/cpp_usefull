#pragma once

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <variant>
#include <unordered_map>
#include <chrono>
#include <iomanip>

namespace tiny_sqlite3 {

using TimePoint = std::chrono::system_clock::time_point;
using Value = std::variant<int, double, std::string, TimePoint>;

class TableWrapper {
private:
    std::vector<std::string> column_names;
    std::vector<std::unordered_map<std::string, Value>> rows;
    std::string time_point_to_string(const TimePoint& tp) const;

public:
    void add_column(const std::string& column_name);
    void add_row(const std::unordered_map<std::string, Value>& row);
    const std::vector<std::string>& get_column_names() const;
    const std::vector<std::unordered_map<std::string, Value>>& get_rows() const;
    void print_table() const;
    size_t get_row_count() const;
    const Value& get_value(size_t row_index, const std::string& column_name) const;
    template<typename Func>
    void for_each_row(Func func) const {
        for (const auto& row : rows) {
            func(row);
        }
    }
};

class SQLiteWrapper {
private:
    sqlite3* db;
    int auto_vacuum_mode;
    static TimePoint string_to_time_point(const std::string& str);
    static int callback(void* data, int argc, char** argv, char** azColName);
    void execute_query(const std::string& query, TableWrapper& table);
    std::string format_value(const Value& value);

public:
    SQLiteWrapper(const std::string& db_name);
    ~SQLiteWrapper();
    void set_auto_vacuum_mode(int mode);
    void insert(const std::string& table, const std::string& columns, const std::vector<Value>& values);
    void remove(const std::string& table, const std::string& condition);
    void update(const std::string& table, const std::string& set_clause, const std::string& condition);
    void select(const std::string& columns, const std::string& table, TableWrapper& result, const std::string& condition = "");
    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();
    void create_table(const std::string& table_name, const std::string& columns_definition);
    void drop_table(const std::string& table_name);
};

} // namespace tiny_sqlite3
