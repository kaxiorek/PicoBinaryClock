#pragma once
#include <functional>
class Button
{
public:
    Button(const uint8_t buttonPin, const std::function<void()>& action);
    void read();

private:
    uint8_t buttonPin_;
    std::function<void()> action_;

    bool wasButtonPressed_{false};
    unsigned long lastDebounceTime_{0};
    bool lastState{false};
};