#include <Arduino.h>

#include "IC_74HC595.hpp"
#include "PicoRTC.hpp"
#include "Button.hpp"

constexpr uint8_t latchPin = 15;
constexpr uint8_t clockPin = 14;
constexpr uint8_t dataPin = 13;
constexpr uint8_t oePin = 8;

constexpr uint8_t ntpPin = 12;
constexpr uint8_t modePin = 11;
constexpr uint8_t changeValPin = 10;
constexpr uint8_t changeBrightnessPin = 9;

IC::Serial74HC595 shiftRegister(latchPin, clockPin, dataPin, oePin);
PicoRTC rtc;

uint8_t currentBrightness = 0;

Button *ntpButton;
Button *changeValButton;
Button *modeButton;
Button *changeBrightnessButton;

void changeBrightness();
void readButtons();

void setPinModes()
{
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(oePin, OUTPUT);

    pinMode(modePin, INPUT_PULLDOWN);
    pinMode(changeValPin, INPUT_PULLDOWN);
    pinMode(changeBrightnessPin, INPUT_PULLDOWN);
    pinMode(ntpPin, INPUT_PULLDOWN);
}

void setup() 
{
    Serial.begin(9600);
    setPinModes();

    currentBrightness = 180;

    ntpButton = new Button(ntpPin, []() {
        Serial.println("Setting time from NTP");
        rtc.setTime(21,35,0,5,11,2022);
    });
    changeBrightnessButton = new Button(changeBrightnessPin, []() {
        Serial.print("Change brigtness to ");
        Serial.println(currentBrightness);
        changeBrightness();
    });
    modeButton = new Button(modePin, []() {
        Serial.println("changing mode is not yet implemented");
    });
    changeValButton = new Button(changeValPin, []() {
        Serial.println("change value is not yet implemented");
    });
    
}

void loop() {
    shiftRegister.setBrightness(currentBrightness);
    shiftRegister.writeValue(rtc.getBinaryTime());
    readButtons();

    delay(10);
}

void readButtons()
{
    ntpButton->read();
    changeBrightnessButton->read();
    modeButton->read();
    changeValButton->read();
}

void changeBrightness()
{
    const uint8_t minVal = 120;
    const uint8_t maxVal = 230;

    currentBrightness += 10;
    if (currentBrightness > maxVal) currentBrightness = minVal;
    shiftRegister.setBrightness(currentBrightness);
}
