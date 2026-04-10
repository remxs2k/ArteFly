#include "serpapi.h"
#include <curl/curl.h>
#include <string>

static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string searchFlights(const std::string& from, const std::string& to, const std::string& date, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    std::string response;

    if(curl) {
        std::string url = "https://serpapi.com/search?engine=google_flights"
                  "&departure_id=" + from +
                  "&arrival_id="   + to +
                  "&outbound_date=" + date +
                  "&type=2" +
                  "&api_key=" + apiKey +
                  "&currency=USD&hl=en";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}