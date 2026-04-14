#pragma once 
#include <sqlite3.h>
#include <string>

using namespace std;

void initDatabase(sqlite3* db);
string getCachedResults(sqlite3* db, const string& from, const string& to, const string& date);
void cacheResult(sqlite3* db, const string& from, const string& to, const string& date, const string& result);