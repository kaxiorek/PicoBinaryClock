#pragma once
#include <functional>
namespace IC
{

class Serial74HC595
{
public:
    Serial74HC595(uint8_t latchPin_, uint8_t clockPin_, uint8_t dataPin_, uint8_t oePin_);
    void writeValue(const uint16_t);
    void setBrightness(const uint8_t);
private:
    void sendDataToRegisters(const uint16_t);

    uint8_t latchPin_;
    uint8_t clockPin_;
    uint8_t dataPin_;
    uint8_t oePin_;
};

}
