#include "Button.h"
#include "config.h"

#include <Arduino.h>

void Button::init()
{
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
}

void Button::poll()
{
    const uint32_t now = millis();

    if (now - lastPollTime < BUTTON_POLL_PERIOD_MS)
    {
        return;
    }

    lastPollTime = now;
    const bool rawState = digitalRead(BUTTON_PIN) == HIGH;

    if (rawState != lastRawState)
    {
        lastRawState = rawState;
        lastRawChangeTime = now;
    }

    if (now - lastRawChangeTime >= BUTTON_DEBOUNCE_MS &&
        stableState != rawState)
    {
        stableState = rawState;

        if (stableState)
        {
            pressStartTime = now;
        }
        else
        {
            buttonState = false;
        }
    }

    if (stableState &&
        !buttonState &&
        now - pressStartTime >= BUTTON_LONG_PRESS_MS)
    {
        buttonState = true;
        Serial.println("Long press detected");
    }
}

bool Button::isLongPressed() const
{
    return buttonState;
}
