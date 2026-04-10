#pragma once
#include <string>
#include <map>

static std::map<std::string, std::string> cityToIATA = {
    {"london", "LHR"},
    {"paris", "CDG"},
    {"rome", "FCO"},
    {"berlin", "BER"},
    {"madrid", "MAD"},
    {"lisbon", "LIS"},
    {"amsterdam", "AMS"},
    {"new york", "JFK"},
    {"los angeles", "LAX"},
    {"chicago", "ORD"},
    {"toronto", "YYZ"},
    {"tokyo", "NRT"},
    {"dubai", "DXB"},
    {"singapore", "SIN"},
    {"sydney", "SYD"},
    {"barcelona", "BCN"},
    {"munich", "MUC"},
    {"frankfurt", "FRA"},
    {"vienna", "VIE"},
    {"zurich", "ZRH"},
    {"brussels", "BRU"},
    {"copenhagen", "CPH"},
    {"stockholm", "ARN"},
    {"oslo", "OSL"},
    {"helsinki", "HEL"},
    {"warsaw", "WAW"},
    {"prague", "PRG"},
    {"budapest", "BUD"},
    {"bucharest", "OTP"},
    {"istanbul", "IST"},
    {"athens", "ATH"}
};

static std::string getIATA(const std::string& city) {
    std::string lower = city;
    for(char& c : lower) c = tolower(c);
    auto it = cityToIATA.find(lower);
    if(it != cityToIATA.end()) return it->second;
    return "";
}