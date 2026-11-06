#include "Button.h"

#include <Arduino.h>

volatile bool Button::changed_ = false;

void IRAM_ATTR Button::onChangeIsr()
{
    // ISR лише фіксує подію. Обробка виконується в superloop.
    changed_ = true;
}

void Button::init()
{
    pinMode(pin_, INPUT_PULLUP);

    raw_ = digitalRead(pin_) == LOW;
    stable_ = raw_;

    attachInterrupt(
        digitalPinToInterrupt(pin_),
        onChangeIsr,
        CHANGE
    );
}

bool Button::update(uint32_t now)
{
    if (consumeChange())
    {
        raw_ = digitalRead(pin_) == LOW;
        lastChangeAt_ = now;
    }

    return debounce(now);
}

bool Button::debounce(uint32_t now)
{
    if (raw_ == stable_ ||
        now - lastChangeAt_ < debounceMs_)
    {
        return false;
    }

    stable_ = raw_;
    return stable_;
}

bool Button::consumeChange()
{
    noInterrupts();
    const bool changed = changed_;
    changed_ = false;
    interrupts();

    return changed;
}
