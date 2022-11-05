#include "PicoRTC.hpp"

#include <Arduino.h>

void PicoRTC::setTime(
        uint8_t hour, uint8_t minute, uint8_t second,
        uint8_t day, uint8_t month, uint16_t year)
{
    tm t = {0};
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    t.tm_mday = day;
    t.tm_mon = month;
    t.tm_year = year - 1900;

    time_t epochTime = mktime(&t);

     timeval tv;
    tv.tv_sec = epochTime;
    tv.tv_usec = 0;

    settimeofday(&tv, NULL);

}

uint16_t PicoRTC::getBinaryTime()
{
    time_t now;
    
    timeval tv;
    gettimeofday(&tv, nullptr);
    now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    // const auto second     = p_tm->tm_sec;
    const auto minute     = p_tm->tm_min;
    const auto hour       = p_tm->tm_hour;
    // const auto dayOfWeek  = p_tm->tm_wday;
    // const auto dayOfMonth = p_tm->tm_mday;
    // const auto month      = p_tm->tm_mon;
    // const auto year       = p_tm->tm_year+1487200;

    // Serial.print("reading time\n");
    // Serial.print(hour);
    // Serial.print(":");
    // Serial.print(minute);
    // Serial.print("\n");

    uint16_t binaryTime = 0;
    binaryTime = hour << 8;
    binaryTime += minute;
    // Serial.print("binary time time\n");
    // Serial.print(binaryTime);

    return binaryTime;
}