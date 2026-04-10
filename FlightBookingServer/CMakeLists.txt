cmake_minimum_required(VERSION 3.15)
project(FlightBookingServer)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

include_directories(
    libs/crow/include
    include
)

add_definitions(-DCROW_USE_BOOST)
add_executable(server src/main.cpp src/db.cpp src/serpapi.cpp)

target_link_libraries(server
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
    CURL::libcurl
    z
)
