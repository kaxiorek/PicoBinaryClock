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

tm* PicoRTC::getTime() 
{
    time_t now;
    timeval tv;
    gettimeofday(&tv, nullptr);
    now = time(nullptr);
    struct tm* p_tm = localtime(&now);

    return p_tm;
}

uint16_t PicoRTC::getBinaryTime()
{
    const auto p_tm = getTime();

    const auto minute = p_tm->tm_min;
    const auto hour = p_tm->tm_hour;

    uint16_t binaryTime = 0;
    binaryTime = hour << 8;
    binaryTime += minute;

    return binaryTime;
}
void PicoRTC::addMinute() 
{
    const auto p_tm = getTime();
    const auto minute     = p_tm->tm_min;

    setTime(p_tm->tm_hour, minute + 1, 0, p_tm->tm_mday, p_tm->tm_mon, p_tm->tm_year + 1900);
}

void PicoRTC::addHour() 
{
    const auto p_tm = getTime();
    const auto hour = p_tm->tm_hour;

    setTime(hour + 1, p_tm->tm_min, 0, p_tm->tm_mday, p_tm->tm_mon, p_tm->tm_year + 1900);  
}
