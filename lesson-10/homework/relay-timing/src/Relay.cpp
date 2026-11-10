#include "Relay.h"

#include <Arduino.h>

volatile bool Relay::contactChanged_ = false;
volatile uint32_t Relay::contactEdgeAtUs_ = 0;

void IRAM_ATTR Relay::onContactChangeIsr()
{
    contactEdgeAtUs_ = micros();
    contactChanged_ = true;
}

void Relay::init()
{
    pinMode(controlPin_, OUTPUT);
    digitalWrite(controlPin_, LOW);

    pinMode(feedbackPin_, INPUT_PULLUP);
    stableClosed_ = digitalRead(feedbackPin_) == LOW;

    attachInterrupt(
        digitalPinToInterrupt(feedbackPin_),
        onContactChangeIsr,
        CHANGE
    );
}

void Relay::setEnabled(bool enabled)
{
    digitalWrite(controlPin_, enabled ? HIGH : LOW);
}

bool Relay::tick(uint32_t nowUs, RelayContactEvent& event)
{
    uint32_t edgeAtUs = 0;
    if (consumeInterrupt(edgeAtUs))
    {
        candidateClosed_ = digitalRead(feedbackPin_) == LOW;
        candidateAtUs_ = edgeAtUs;
        candidatePending_ = true;
    }

    return debounce(nowUs, event);
}

bool Relay::debounce(uint32_t nowUs, RelayContactEvent& event)
{

    if (!candidatePending_ || nowUs - candidateAtUs_ < debounceUs_)
    {
        return false;
    }

    const bool currentClosed = digitalRead(feedbackPin_) == LOW;
    candidatePending_ = false;

    if (currentClosed != candidateClosed_ || currentClosed == stableClosed_)
    {
        return false;
    }

    stableClosed_ = currentClosed;
    event = {currentClosed, candidateAtUs_};
    return true;
}

bool Relay::consumeInterrupt(uint32_t& edgeAtUs)
{
    noInterrupts();
    const bool changed = contactChanged_;
    edgeAtUs = contactEdgeAtUs_;
    contactChanged_ = false;
    interrupts();
    return changed;
}
