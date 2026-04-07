#include "crow.h"
#include "json.hpp"
#include "db.h"
#include <sqlite3.h>

using namespace std;
using json = nlohmann::json;

int main(){

    sqlite3* db;

    int rc = sqlite3_open("flights.db", &db);

    initDatabase(db);
    
    crow::SimpleApp app;

    CROW_ROUTE(app, "/ping")
    ([]{
        return crow::response(200, "Fuck off!\n");
    });

    CROW_ROUTE(app, "/search")
    ([&db](const crow::request& req){
        auto from = req.url_params.get("from");
        auto to = req.url_params.get("to");

        if(!from){
            return crow::response(400, "Missing");
        }

        const char* query = R"(
            SELECT id, from_city, to_city, departure, arrival, price, airline, seats
            FROM flights
            WHERE from_city = ? AND to_city = ?
        )";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, from, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, to,   -1, SQLITE_STATIC);

        json result;
        result["flights"] = json::array();

        while(sqlite3_step(stmt) == SQLITE_ROW){
            json flight;
            flight["id"] = sqlite3_column_int(stmt, 0);
            flight["from"] = (const char*)sqlite3_column_text(stmt, 1);
            flight["to"] = (const char*)sqlite3_column_text(stmt, 2);
            flight["departure"] = (const char*)sqlite3_column_text(stmt, 3);
            flight["arrival"] = (const char*)sqlite3_column_text(stmt, 4);
            flight["price"] = sqlite3_column_double(stmt, 5);
            flight["airline"] = (const char*) sqlite3_column_text(stmt, 6);
            flight["seats"] = sqlite3_column_int(stmt, 7);
            result["flights"].push_back(flight);
        }

        sqlite3_finalize(stmt);

    });

    CROW_ROUTE(app, "/booking").methods("POST"_method)
    ([&db](const crow::request& req){

        auto body = json::parse(req.body, nullptr, false);

        string flight_id = body["flight_id"];
        string passenger_name = body["passenger_name"];
        string email = body["email"];
        int seats = body["seats"];

        time_t now = time(0);
        char date[20];

        strftime(date, sizeof(date), "%d-%m-%Y", localtime(&now));

        const char* query = R"(
            INSERT INTO bookings(flight_id, passenger_name, email, seats, booking_date)
            VALUES(?, ?, ?, ?, ?)
        )";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, flight_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, passenger_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, seats);
        sqlite3_bind_text(stmt, 5, date, -1, SQLITE_STATIC);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(rc != SQLITE_DONE){
            return crow::response(500, "BOOKING FAILED!");
        }

        json result = {
            {"message", "Booking successful"},
            {"booking_date", date}
        };

        crow::response res(result.dump());
        res.add_header("Content-Type", "application/json");
        return res;
    });
    

    

    app.port(8080).multithreaded().run();
    sqlite3_close(db);
    return 0;
}