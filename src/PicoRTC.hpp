#pragma once

#include <time.h>
#include <sys/time.h>

class PicoRTC
{
public:
    void setTime(
        uint8_t hour, uint8_t minute, uint8_t second,
        uint8_t day, uint8_t month, uint16_t year);

    uint16_t getBinaryTime();

};