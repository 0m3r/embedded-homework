#include "RelayContact.h"

#include <Arduino.h>

volatile bool RelayContact::changed_ = false;
volatile uint32_t RelayContact::changedAtUs_ = 0;

void IRAM_ATTR RelayContact::onChangeIsr()
{
    changedAtUs_ = micros();
    changed_ = true;
}

void RelayContact::init()
{
    pinMode(pin_, INPUT_PULLUP);
    closed_ = digitalRead(pin_) == LOW;

    attachInterrupt(
        digitalPinToInterrupt(pin_),
        onChangeIsr,
        CHANGE
    );
}

bool RelayContact::tick(uint32_t nowUs)
{
    uint32_t changedAtUs = 0;
    if (consumeInterrupt(changedAtUs))
    {
        pendingClosed_ = digitalRead(pin_) == LOW;
        pendingAtUs_ = changedAtUs;
        pending_ = true;
    }

    if (!debounce(nowUs))
    {
        return false;
    }

    return true;
}

bool RelayContact::isClosed() const
{
    return closed_;
}

uint32_t RelayContact::changedAtUs() const
{
    return pendingAtUs_;
}

bool RelayContact::consumeInterrupt(uint32_t& changedAtUs)
{
    noInterrupts();
    const bool changed = changed_;
    changedAtUs = changedAtUs_;
    changed_ = false;
    interrupts();
    return changed;
}

bool RelayContact::debounce(uint32_t nowUs)
{
    if (!pending_ || nowUs - pendingAtUs_ < debounceUs_)
    {
        return false;
    }

    const bool currentClosed = digitalRead(pin_) == LOW;
    pending_ = false;

    if (currentClosed != pendingClosed_ || currentClosed == closed_)
    {
        return false;
    }

    closed_ = currentClosed;
    return true;
}
