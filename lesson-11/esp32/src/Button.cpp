#include "Button.h"

#include <Arduino.h>

void Button::init()
{
    pinMode(pin_, INPUT_PULLUP);
    debouncer_.init(read());
}

bool Button::update()
{
    const bool pressed = read();
    return debouncer_.update(pressed) && debouncer_.state();
}

bool Button::read() const
{
    return digitalRead(pin_) == LOW;
}
