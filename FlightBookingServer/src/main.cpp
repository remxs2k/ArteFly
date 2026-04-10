#include "crow.h"
#include "json.hpp"
#include "db.h"
#include "airports.h"
#include "serpapi.h"
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
        auto to   = req.url_params.get("to");
        auto date = req.url_params.get("date");

        if(!from || !to || !date){
            return crow::response(400, "Missing parameters");
        }


        std::string fromCode = getIATA(from);
        std::string toCode   = getIATA(to);

        if(fromCode.empty() || toCode.empty()){
            return crow::response(400, "Unknown city name");
        }

    
        const char* apiKey = std::getenv("SERPAPI_KEY");
        if(!apiKey){
            return crow::response(500, "API key not configured");
        }
        //std::string rawResponse = searchFlights(fromCode, toCode, date, apiKey);

        std::string rawResponse = searchFlights(fromCode, toCode, date, apiKey);
        std::cout << "SerpApi raw response: " << rawResponse << std::endl;

        // Parse the response
        auto serpData = json::parse(rawResponse, nullptr, false);
        if(serpData.is_discarded()){
            return crow::response(500, "Failed to parse flight data");
        }

        // Extract best_flights or other_flights
        json result;
        result["flights"] = json::array();

        auto extractFlights = [&](const std::string& key){
            if(!serpData.contains(key)) return;
            for(auto& f : serpData[key]){
                json flight;
                flight["price"]    = f.value("price", 0);
                flight["airline"]  = f["flights"][0].value("airline", "Unknown");
                flight["from"]     = f["flights"][0].value("departure_airport", json::object()).value("name", from);
                flight["to"]       = f["flights"][0].value("arrival_airport", json::object()).value("name", to);
                flight["departure"]= f["flights"][0].value("departure_airport", json::object()).value("time", "");
                flight["arrival"]  = f["flights"][0].value("arrival_airport", json::object()).value("time", "");
                flight["duration"] = f.value("total_duration", 0);
                result["flights"].push_back(flight);
            }
        };

        extractFlights("best_flights");
        extractFlights("other_flights");

        crow::response res(result.dump());
        res.add_header("Content-Type", "application/json");
        return res;
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