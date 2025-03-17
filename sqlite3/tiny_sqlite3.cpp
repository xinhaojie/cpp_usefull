/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-12 16:35:12
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-13 10:32:05
* @FilePath     : /cpp_usefull/sqlite3/tiny_sqlite3.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_sqlite3.hxx"

namespace tiny_sqlite3 {

// 自定义的 SQLite 异常类实现
SQLiteException::SQLiteException(const std::string& message) : msg(message) {}

const char* SQLiteException::what() const noexcept {
    return msg.c_str();
}

// SQLite3 封装类实现
SQLiteWrapper::SQLiteWrapper(const std::string& dbName, int autoVacuumMode) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        throw SQLiteException("Can't open database: " + std::string(sqlite3_errmsg(db)));
    }
    // 设置 auto_vacuum 模式
    std::string autoVacuumSql = "PRAGMA auto_vacuum = " + std::to_string(autoVacuumMode) + ";";
    execute(autoVacuumSql);
}

SQLiteWrapper::~SQLiteWrapper() {
    if (db) {
        sqlite3_close(db);
    }
}

void SQLiteWrapper::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw SQLiteException(error);
    }
}

void SQLiteWrapper::createTable(const std::string& tableName, const std::string& columnsDefinition) {
    std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columnsDefinition + ");";
    execute(sql);
}

void SQLiteWrapper::dropTable(const std::string& tableName) {
    std::string sql = "DROP TABLE IF EXISTS " + tableName + ";";
    execute(sql);
}

void SQLiteWrapper::insert(const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& values) {
    std::string sql = "INSERT INTO " + table + " (";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (i > 0) {
            sql += ", ";
        }
        sql += columns[i];
    }
    sql += ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            sql += ", ";
        }
        sql += "'" + values[i] + "'";
    }
    sql += ");";
    execute(sql);
}

void SQLiteWrapper::remove(const std::string& table, const std::string& condition) {
    std::string sql = "DELETE FROM " + table + " WHERE " + condition + ";";
    execute(sql);
    // 调用增量碎片整理
    execute("PRAGMA incremental_vacuum;");
}

void SQLiteWrapper::batchRemove(const std::string& table, const std::vector<std::string>& conditions) {
    for (const auto& condition : conditions) {
        remove(table, condition);
    }
}

void SQLiteWrapper::update(const std::string& table, const std::string& setClause, const std::string& condition) {
    std::string sql = "UPDATE " + table + " SET " + setClause + " WHERE " + condition + ";";
    execute(sql);
}

void SQLiteWrapper::batchUpdate(const std::string& table, const std::vector<std::pair<std::string, std::string>>& updates) {
    for (const auto& updatePair : updates) {
        update(table, updatePair.first, updatePair.second);
    }
}

std::vector<std::vector<std::string>> SQLiteWrapper::select(const std::string& table, const std::vector<std::string>& columns,
                                                            const std::string& condition, const std::string& orderBy) {
    std::string sql = "SELECT ";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (i > 0) {
            sql += ", ";
        }
        sql += columns[i];
    }
    sql += " FROM " + table;
    if (!condition.empty()) {
        sql += " WHERE " + condition;
    }
    if (!orderBy.empty()) {
        sql += " ORDER BY " + orderBy;
    }
    sql += ";";

    std::vector<std::vector<std::string>> result;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw SQLiteException("SQL prepare error: " + std::string(sqlite3_errmsg(db)));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
            row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
        }
        result.push_back(row);
    }

    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        throw SQLiteException("SQL finalize error: " + std::string(sqlite3_errmsg(db)));
    }

    return result;
}

std::vector<std::vector<std::vector<std::string>>> SQLiteWrapper::batchSelect(const std::string& table, const std::vector<std::string>& columns,
                                                                            const std::vector<std::string>& conditions,
                                                                            const std::string& orderBy) {
    std::vector<std::vector<std::vector<std::string>>> results;
    for (const auto& condition : conditions) {
        results.push_back(select(table, columns, condition, orderBy));
    }
    return results;
}

}