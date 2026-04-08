#include <sqlite3.h>
#include <iostream>

using namespace std;


void initDatabase(sqlite3* db){
    const char* createTable = R"(
        CREATE TABLE IF NOT EXISTS flights (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            from_city TEXT NOT NULL,
            to_city TEXT NOT NULL,
            departure TEXT NOT NULL,
            arrival TEXT NOT NULL,
            price REAL NOT NULL,
            airline TEXT NOT NULL,
            seats INTEGER NOT NULL

        );
    )";

    int rc = sqlite3_exec(db, createTable, nullptr, nullptr, nullptr);

    const char* checkEmpty = "SELECT COUNT(*) FROM flights";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, checkEmpty, -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    if(count > 0) return;

    const char* insertFlights = R"(
        INSERT INTO flights(from_city, to_city, departure, arrival, price, airline, seats) VALUES
        ('London',    'Paris',     '01:00', '03:00', 200.0, 'Lufthansa', 60),
        ('London',    'Paris',     '08:00', '10:00', 120.00, 'WizzAir',  50),
        ('London',    'Paris',     '14:00', '16:00',  95.00, 'EasyJet',    30),
        ('Paris',     'Rome',      '09:00', '11:30', 150.00, 'Tarom',   40),
        ('Rome',      'Berlin',    '13:00', '15:30', 110.00, 'Lufthansa',  60),
        ('Berlin',    'Madrid',    '07:00', '10:00', 200.00, 'WizzAir',     45),
        ('Madrid',    'Lisbon',    '11:00', '12:00',  60.00, 'KLM',    55),
        ('Amsterdam', 'London',    '06:00', '07:00',  80.00, 'KLM',        35),
        ('London',    'Amsterdam', '16:00', '17:00',  85.00, 'KLM',        35);
    )";

    rc = sqlite3_exec(db, insertFlights, nullptr, nullptr, nullptr);

    const char* bookings = R"(
        CREATE TABLE IF NOT EXISTS bookings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            flight_id TEXT NOT NULL,
            passenger_name TEXT NOT NULL,
            email TEXT NOT NULL,
            seats INTEGER NOT NULL,
            booking_date NOT NULL,
            FOREIGN KEY(flight_id) REFERENCES flights(id)
        );
    )";

    sqlite3_exec(db, bookings, nullptr, nullptr, nullptr);

}