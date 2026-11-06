#pragma once

#include <Arduino.h>

class Button
{
private:
    uint32_t lastPollTime = 0;
    uint32_t lastRawChangeTime = 0;
    uint32_t pressStartTime = 0;

    bool lastRawState = false;
    bool stableState = false;
    bool buttonState = false;

public:
    void init();
    void poll();
    bool isLongPressed() const;
};
