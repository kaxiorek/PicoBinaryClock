#include <Arduino.h>

#include "IC_74HC595.hpp"
#include "PicoRTC.hpp"
#include "Button.hpp"
#include "State.hpp"

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
State state = State::Normal;

Button *ntpButton;
Button *changeValButton;
Button *modeButton;
Button *changeBrightnessButton;

void changeBrightness();
void readButtons();
void changeMode();
void changeTime();

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
        changeMode();
    });
    changeValButton = new Button(changeValPin, []() {
        changeTime();
    });
}

unsigned long timeNow = 0;
unsigned long blinkingTime = 500;
bool shouldBlink = false;

void loop() {
    shiftRegister.setBrightness(currentBrightness);
    if (state == State::Normal)
        shiftRegister.writeValue(rtc.getBinaryTime());
    else if (state == State::ManualModeMinutes || state == State::ManualModeHours)
    {
        if ((millis() - timeNow) > blinkingTime)
        {
            shouldBlink = !shouldBlink;
            timeNow = millis();        
        }
        auto binaryTime = rtc.getBinaryTime();
        
        if (shouldBlink)
        {
            binaryTime &= state == State::ManualModeMinutes ? 0xFF00 : 0xFF;
            shiftRegister.writeValue(binaryTime);
        }    
        else
            shiftRegister.writeValue(binaryTime); 
    }
    
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

void changeMode()
{
    if (state == State::Normal)
    {
        Serial.println("Change mode to ManualModeMinutes");
        state = State::ManualModeMinutes;
    }
    else if (state == State::ManualModeMinutes)
    {
        Serial.println("Change mode to ManualModeHours");
        state = State::ManualModeHours;
    }
    else if (state == State::ManualModeHours)
    {
        Serial.println("Change mode to Normal");
        state = State::Normal;
    }
}

void changeTime()
{
    if (state == State::ManualModeMinutes) 
        rtc.addMinute();
    else if (state == State::ManualModeHours)
        rtc.addHour();
}