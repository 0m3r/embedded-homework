#include "Button.h"

#include <Arduino.h>

volatile bool Button::interruptEvent_ = false;

void IRAM_ATTR Button::onChangeIsr()
{
    // ISR лише фіксує подію. Обробка виконується в superloop.
    interruptEvent_ = true;
}

void Button::init()
{
    pinMode(pin_, INPUT_PULLUP);

    rawState_ = digitalRead(pin_) == HIGH;
    stableState_ = rawState_;

    attachInterrupt(
        digitalPinToInterrupt(pin_),
        onChangeIsr,
        CHANGE
    );
}

void Button::update(uint32_t now)
{
    if (takeInterruptEvent())
    {
        rawState_ = digitalRead(pin_) == HIGH;
        lastChangeAt_ = now;
    }

    if (rawState_ != stableState_ &&
        now - lastChangeAt_ >= debounceMs_)
    {
        stableState_ = rawState_;

        // INPUT_PULLUP: LOW означає підтверджене натискання.
        if (!stableState_)
        {
            pressedEvent_ = true;
        }
    }
}

bool Button::wasPressed()
{
    const bool result = pressedEvent_;
    pressedEvent_ = false;
    return result;
}

bool Button::takeInterruptEvent()
{
    noInterrupts();
    const bool eventDetected = interruptEvent_;
    interruptEvent_ = false;
    interrupts();

    return eventDetected;
}
