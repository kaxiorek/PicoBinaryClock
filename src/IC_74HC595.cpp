#include "IC_74HC595.hpp"

#include <Arduino.h>

namespace IC
{

Serial74HC595::Serial74HC595(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t oePin)
    : latchPin_(latchPin), clockPin_(clockPin), dataPin_(dataPin), oePin_(oePin) {}

void Serial74HC595::setBrightness(const uint8_t val) 
{
    analogWrite(oePin_, val);    
}

void Serial74HC595::writeValue(const uint16_t val)
{
    digitalWrite(latchPin_, LOW);   
    sendDataToRegisters(val);
    digitalWrite(latchPin_, HIGH);
}

void Serial74HC595::sendDataToRegisters(const uint16_t val)
{
    for (uint16_t i = 0; i < 16; i++)
    {
        digitalWrite(dataPin_, !!(val & (1 << i)) ? HIGH : LOW);
        // digitalWrite(dataPin_, !!(val & (1 << (15 - i))) ? HIGH : LOW);
        digitalWrite(clockPin_, HIGH);
        digitalWrite(clockPin_, LOW);		
	}   
}

}
