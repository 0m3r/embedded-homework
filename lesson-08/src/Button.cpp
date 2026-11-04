#include "Button.h"
#include "config.h"

#include <Arduino.h>

void Button::init()
{
    pinMode(BTN, INPUT_PULLDOWN);
}

void Button::poll()
{
    if (millis() - lastTimeChangeBtn > BTN_PERIOD_MS)
    {
        lastTimeChangeBtn = millis();

        uint16_t btn_value = digitalRead(BTN);

        if (btn_value == HIGH)
        {
            if (btnTimekeeper == 0)
            {
                btnTimekeeper = millis();
            }
            else if (millis() - btnTimekeeper > BTN_LONG_PRESS_MS)
            {
                buttonState = true;

                Serial.printf(
                    "btn %lu\n",
                    static_cast<unsigned long>(millis())
                );
            }
        }
        else
        {
            buttonState = false;
            btnTimekeeper = 0;
        }
    }
}

bool Button::get_state()
{
    return buttonState;
}