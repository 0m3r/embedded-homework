#include "Relay.h"

#include <Arduino.h>

volatile bool Relay::contactChanged_ = false;
volatile uint32_t Relay::edgeAtUs_ = 0;

void IRAM_ATTR Relay::onContactChangeIsr()
{
    edgeAtUs_ = micros();
    contactChanged_ = true;
}

void Relay::init()
{
    pinMode(controlPin_, OUTPUT);
    off();

    pinMode(feedbackPin_, INPUT_PULLUP);
    stableClosed_ = digitalRead(feedbackPin_) == LOW;

    attachInterrupt(
        digitalPinToInterrupt(feedbackPin_),
        onContactChangeIsr,
        CHANGE
    );
}

void Relay::on()
{
    enabled_ = true;
    digitalWrite(controlPin_, HIGH);
}

void Relay::off()
{
    enabled_ = false;
    digitalWrite(controlPin_, LOW);
}

void Relay::toggle()
{
    if (enabled_)
    {
        off();
    }
    else
    {
        on();
    }
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

    if (!debounce(nowUs))
    {
        return false;
    }

    event = {stableClosed_, candidateAtUs_};
    return true;
}

bool Relay::debounce(uint32_t nowUs)
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
    return true;
}

bool Relay::consumeInterrupt(uint32_t& edgeAtUs)
{
    noInterrupts();
    const bool changed = contactChanged_;
    edgeAtUs = edgeAtUs_;
    contactChanged_ = false;
    interrupts();
    return changed;
}
