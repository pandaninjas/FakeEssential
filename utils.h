//
// Created by pandaninjas on 12/6/22.
//


#include <cstdio>
#include <string>

#ifndef FAKE_ESSENTIALS_UTILS_H
const char path_separator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif
size_t write_into_string(void *contents, size_t size, size_t nmemb, std::string *s);
std::string get_uuid(const std::string& username);
std::string split_and_get_last(char * to_split, char splitter);
#define FAKE_ESSENTIALS_UTILS_H

#endif //FAKE_ESSENTIALS_UTILS_H
