#include "../include/time_utils.hpp"

#include <ctime>
#include <cstdio>

#include <chrono>
using namespace std::chrono;

string current_time_string(){
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    tm local_time = *localtime(&t);

    char buff[9];
    snprintf(buff, sizeof(buff), "%02d:%02d:%02d",
             local_time.tm_hour,
             local_time.tm_min,
             local_time.tm_sec);

    return string(buff);
}