#include <curl/curl.h>
#include <iostream>
#include "json/json.hpp"

size_t write_into_string(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    try
    {
        s->append((char*)contents, newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }
    return newLength;
}

std::string split_and_get_last(char * to_split, char splitter) {
    std::string last;
    for (size_t i = strlen(to_split) - 1; i < strlen(to_split); i--) {
        if (to_split[i] == splitter) {
            break;
        }
        last.insert(0, 1, to_split[i]);
    }
    return last;
}


std::string get_uuid(const std::string& username) {
    std::string http_response;
    CURL *curl;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                         ("https://api.mojang.com/users/profiles/minecraft/" + username).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_into_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_response);
    }
    curl_easy_perform(curl);
    nlohmann::json api_response = nlohmann::json::parse(http_response);
    curl_easy_cleanup(curl);
    return api_response["id"].get<std::string>();
}