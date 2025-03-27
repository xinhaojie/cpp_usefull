/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:35:12
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-27 13:56:36
* @FilePath     : /cpp_usefull/sqlite3/tiny_sqlite3.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_sqlite3.hpp"
namespace tiny_sqlite3 {

std::string TableWrapper::time_point_to_string(const TimePoint& tp) const {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void TableWrapper::add_column(const std::string& column_name) {
    column_names.push_back(column_name);
}

void TableWrapper::add_row(const std::unordered_map<std::string, Value>& row) {
    rows.push_back(row);
}

const std::vector<std::string>& TableWrapper::get_column_names() const {
    return column_names;
}

const std::vector<std::unordered_map<std::string, Value>>& TableWrapper::get_rows() const {
    return rows;
}

void TableWrapper::print_table() const {
    for (const auto& column : column_names) {
        std::cout << column << "\t";
    }
    std::cout << std::endl;

    for (const auto& row : rows) {
        for (const auto& column : column_names) {
            if (row.find(column) != row.end()) {
                const auto& value = row.at(column);
                if (std::holds_alternative<int>(value)) {
                    std::cout << std::get<int>(value) << "\t";
                } else if (std::holds_alternative<double>(value)) {
                    std::cout << std::get<double>(value) << "\t";
                } else if (std::holds_alternative<std::string>(value)) {
                    std::cout << std::get<std::string>(value) << "\t";
                } else if (std::holds_alternative<TimePoint>(value)) {
                    std::cout << time_point_to_string(std::get<TimePoint>(value)) << "\t";
                }
            } else {
                std::cout << "NULL\t";
            }
        }
        std::cout << std::endl;
    }
}

size_t TableWrapper::get_row_count() const {
    return rows.size();
}

const Value& TableWrapper::get_value(size_t row_index, const std::string& column_name) const {
    if (row_index >= rows.size()) {
        throw std::out_of_range("Row index out of range");
    }
    const auto& row = rows[row_index];
    auto it = row.find(column_name);
    if (it == row.end()) {
        throw std::invalid_argument("Column name not found");
    }
    return it->second;
}

TimePoint SQLiteWrapper::string_to_time_point(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

int SQLiteWrapper::callback(void* data, int argc, char** argv, char** azColName) {
    TableWrapper* table = static_cast<TableWrapper*>(data);
    if (table->get_column_names().empty()) {
        for (int i = 0; i < argc; ++i) {
            table->add_column(azColName[i]);
        }
    }
    std::unordered_map<std::string, Value> row;
    for (int i = 0; i < argc; ++i) {
        if (argv[i]) {
            try {
                int int_val = std::stoi(argv[i]);
                row[azColName[i]] = int_val;
            } catch (const std::invalid_argument&) {
                try {
                    double double_val = std::stod(argv[i]);
                    row[azColName[i]] = double_val;
                } catch (const std::invalid_argument&) {
                    try {
                        TimePoint time_val = string_to_time_point(argv[i]);
                        row[azColName[i]] = time_val;
                    } catch (const std::exception&) {
                        row[azColName[i]] = std::string(argv[i]);
                    }
                }
            }
        }
    }
    table->add_row(row);
    return 0;
}

void SQLiteWrapper::execute_query(const std::string& query, TableWrapper& table) {
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, query.c_str(), callback, &table, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        throw std::runtime_error("SQL execution failed");
    }
}

std::string SQLiteWrapper::format_value(const Value& value) {
    std::ostringstream oss;
    if (std::holds_alternative<int>(value)) {
        oss << std::get<int>(value);
    } else if (std::holds_alternative<double>(value)) {
        oss << std::get<double>(value);
    } else if (std::holds_alternative<std::string>(value)) {
        oss << "'";
        const std::string& str = std::get<std::string>(value);
        for (char c : str) {
            if (c == '\'') {
                oss << '\'';
            }
            oss << c;
        }
        oss << "'";
    } else if (std::holds_alternative<TimePoint>(value)) {
        std::time_t t = std::chrono::system_clock::to_time_t(std::get<TimePoint>(value));
        oss << "'" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "'";
    }
    return oss.str();
}

SQLiteWrapper::SQLiteWrapper(const std::string& db_name) : auto_vacuum_mode(0) {
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Database opening failed");
    }
}

SQLiteWrapper::~SQLiteWrapper() {
    sqlite3_close(db);
}

void SQLiteWrapper::set_auto_vacuum_mode(int mode) {
    auto_vacuum_mode = mode;
    std::string query = "PRAGMA auto_vacuum = " + std::to_string(auto_vacuum_mode) + ";";
    execute_query(query, *(new TableWrapper()));
}

void SQLiteWrapper::insert(const std::string& table, const std::string& columns, const std::vector<Value>& values) {
    std::ostringstream oss;
    oss << "INSERT INTO " << table << " (" << columns << ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << format_value(values[i]);
    }
    oss << ");";
    execute_query(oss.str(), *(new TableWrapper()));
}

void SQLiteWrapper::remove(const std::string& table, const std::string& condition) {
    std::string query = "DELETE FROM " + table + " WHERE " + condition + ";";
    execute_query(query, *(new TableWrapper()));
}

void SQLiteWrapper::update(const std::string& table, const std::string& set_clause, const std::string& condition) {
    std::string query = "UPDATE " + table + " SET " + set_clause + " WHERE " + condition + ";";
    execute_query(query, *(new TableWrapper()));
}

void SQLiteWrapper::select(const std::string& columns, const std::string& table, TableWrapper& result, const std::string& condition) {
    std::string query = "SELECT " + columns + " FROM " + table;
    if (!condition.empty()) {
        query += " WHERE " + condition;
    }
    query += ";";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("SQL preparation failed");
    }

    int column_count = sqlite3_column_count(stmt);
    if (result.get_column_names().empty()) {
        for (int i = 0; i < column_count; ++i) {
            result.add_column(sqlite3_column_name(stmt, i));
        }
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::unordered_map<std::string, Value> row;
        for (int i = 0; i < column_count; ++i) {
            const char* value_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            if (value_text) {
                try {
                    int int_val = std::stoi(value_text);
                    row[result.get_column_names()[i]] = int_val;
                } catch (const std::invalid_argument&) {
                    try {
                        double double_val = std::stod(value_text);
                        row[result.get_column_names()[i]] = double_val;
                    } catch (const std::invalid_argument&) {
                        try {
                            TimePoint time_val = string_to_time_point(value_text);
                            row[result.get_column_names()[i]] = time_val;
                        } catch (const std::exception&) {
                            row[result.get_column_names()[i]] = std::string(value_text);
                        }
                    }
                }
            }
        }
        result.add_row(row);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        throw std::runtime_error("SQL execution failed");
    }

    sqlite3_finalize(stmt);
}

void SQLiteWrapper::begin_transaction() {
    execute_query("BEGIN TRANSACTION;", *(new TableWrapper()));
}

void SQLiteWrapper::commit_transaction() {
    execute_query("COMMIT TRANSACTION;", *(new TableWrapper()));
}

void SQLiteWrapper::rollback_transaction() {
    execute_query("ROLLBACK TRANSACTION;", *(new TableWrapper()));
}

void SQLiteWrapper::create_table(const std::string& table_name, const std::string& columns_definition) {
    std::string query = "CREATE TABLE IF NOT EXISTS " + table_name + " (" + columns_definition + ");";
    execute_query(query, *(new TableWrapper()));
}

void SQLiteWrapper::drop_table(const std::string& table_name) {
    std::string query = "DROP TABLE IF EXISTS " + table_name + ";";
    execute_query(query, *(new TableWrapper()));
}

} // namespace tiny_sqlite3