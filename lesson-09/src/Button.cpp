#include "Button.h"

#include <Arduino.h>

volatile bool Button::interruptPending_ = false;

void IRAM_ATTR Button::onChangeIsr()
{
    // ISR лише фіксує подію. Обробка виконується в superloop.
    interruptPending_ = true;
}

void Button::init()
{
    pinMode(pin_, INPUT_PULLUP);

    rawPressed_ = digitalRead(pin_) == LOW;
    stablePressed_ = rawPressed_;

    attachInterrupt(
        digitalPinToInterrupt(pin_),
        onChangeIsr,
        CHANGE
    );
}

bool Button::update(uint32_t now)
{
    if (consumeInterruptEvent())
    {
        rawPressed_ = digitalRead(pin_) == LOW;
        lastChangeAt_ = now;
    }

    return debounce(now);
}

bool Button::debounce(uint32_t now)
{
    if (rawPressed_ == stablePressed_ ||
        now - lastChangeAt_ < debounceMs_)
    {
        return false;
    }

    stablePressed_ = rawPressed_;
    return stablePressed_;
}

bool Button::consumeInterruptEvent()
{
    noInterrupts();
    const bool wasPending = interruptPending_;
    interruptPending_ = false;
    interrupts();

    return wasPending;
}
