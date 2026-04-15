#include <sqlite3.h>
#include <iostream>
#include "db.h"

using namespace std;


void initDatabase(sqlite3* db){
    const char* cacheTable = R"(
        CREATE TABLE IF NOT EXISTS cache (
            id  INTEGER PRIMARY KEY AUTOINCREMENT,
            from_city TEXT NOT NULL,
            to_city TEXT NOT NULL,
            date TEXT NOT NULL,
            result TEXT NOT NULL,
            time TEXT NOT NULL
        );
        
    )";
    sqlite3_exec(db, cacheTable, nullptr, nullptr, nullptr);
}



string getCachedResults(sqlite3* db, const string& from_city, const string& to_city, const string& date){
    time_t refresh = time(0) - 900; // 15 min

    const char* query = R"(
        SELECT result FROM cache
        WHERE from_city = ? AND to_city = ? AND date = ? AND time > ?
        ORDER BY time DESC LIMIT 1
    )";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, from_city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, to_city.c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 4, (int)refresh);

    string result;
    if(sqlite3_step(stmt) == SQLITE_ROW){
        result = (const char*)sqlite3_column_text(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return result;
}

void cacheResult(sqlite3* db, const string& from, const string& to, const string& date, const string& result) {
    const char* query = R"(
        INSERT INTO cache(from_city, to_city, date, result, time)
        VALUES(?, ?, ?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, from.c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, to.c_str(),     -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date.c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, result.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 5, (int)time(0));

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}