#include "RelayContact.h"

#include <Arduino.h>

volatile bool RelayContact::changed_ = false;
volatile uint32_t RelayContact::edgeAtUs_ = 0;

void IRAM_ATTR RelayContact::onChangeIsr()
{
    edgeAtUs_ = micros();
    changed_ = true;
}

void RelayContact::init()
{
    pinMode(pin_, INPUT_PULLUP);
    stableClosed_ = digitalRead(pin_) == LOW;

    attachInterrupt(
        digitalPinToInterrupt(pin_),
        onChangeIsr,
        CHANGE
    );
}

bool RelayContact::tick(uint32_t nowUs)
{
    uint32_t edgeAtUs = 0;
    if (consumeInterrupt(edgeAtUs))
    {
        candidateClosed_ = digitalRead(pin_) == LOW;
        candidateAtUs_ = edgeAtUs;
        candidatePending_ = true;
    }

    if (!debounce(nowUs))
    {
        return false;
    }

    changedAtUs_ = candidateAtUs_;
    return true;
}

bool RelayContact::isClosed() const
{
    return stableClosed_;
}

uint32_t RelayContact::changedAtUs() const
{
    return changedAtUs_;
}

bool RelayContact::consumeInterrupt(uint32_t& edgeAtUs)
{
    noInterrupts();
    const bool changed = changed_;
    edgeAtUs = edgeAtUs_;
    changed_ = false;
    interrupts();
    return changed;
}

bool RelayContact::debounce(uint32_t nowUs)
{
    if (!candidatePending_ || nowUs - candidateAtUs_ < debounceUs_)
    {
        return false;
    }

    const bool currentClosed = digitalRead(pin_) == LOW;
    candidatePending_ = false;

    if (currentClosed != candidateClosed_ || currentClosed == stableClosed_)
    {
        return false;
    }

    stableClosed_ = currentClosed;
    return true;
}
