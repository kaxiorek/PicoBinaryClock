#include "Button.hpp"

#include <Arduino.h>

constexpr unsigned long debounceDelay = 50;

Button::Button(const uint8_t buttonPin, const std::function<void()>& action)
    : buttonPin_(buttonPin), action_(action) {}

void Button::read() 
{
    const auto ntpButton = digitalRead(buttonPin_);
    bool currentState = false;
    if (ntpButton == HIGH)
    {
        currentState = true;
    }
    else if (ntpButton == LOW && wasButtonPressed_)
    {
        wasButtonPressed_ = false;
    }

    if (lastState != currentState)
    {
        lastDebounceTime_ = millis();
    }
    if ((millis() - lastDebounceTime_) > debounceDelay)
    {
        if (currentState == lastState)
        {
            if (ntpButton == HIGH && not wasButtonPressed_)
            {
                action_();
                wasButtonPressed_ = true;
            }
        }
    }

    lastState = currentState;
}
