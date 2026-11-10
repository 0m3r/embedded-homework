#include "RelayTester.h"

#include <Arduino.h>

volatile bool RelayTester::contactChanged_ = false;
volatile uint32_t RelayTester::contactEdgeAtUs_ = 0;

void IRAM_ATTR RelayTester::onContactChangeIsr()
{
    // ISR only captures the latest edge. Debounce runs in the superloop.
    contactEdgeAtUs_ = micros();
    contactChanged_ = true;
}

void RelayTester::init(uint32_t nowUs)
{
    pinMode(controlPin_, OUTPUT);
    digitalWrite(controlPin_, LOW);

    pinMode(contactPin_, INPUT_PULLUP);
    stableClosed_ = digitalRead(contactPin_) == LOW;
    stateStartedAtUs_ = nowUs;

    attachInterrupt(
        digitalPinToInterrupt(contactPin_),
        onContactChangeIsr,
        CHANGE
    );
}

void RelayTester::update(uint32_t nowUs)
{
    bool closed = false;
    uint32_t changedAtUs = 0;

    if (updateContact(nowUs, closed, changedAtUs))
    {
        processContact(closed, changedAtUs, nowUs);
    }

    switch (state_)
    {
        case State::SettlingBeforeOn:
        case State::HoldingOff:
            if (nowUs - stateStartedAtUs_ >= settleUs_)
            {
                setRelay(true, nowUs);
            }
            break;

        case State::HoldingOn:
            if (nowUs - stateStartedAtUs_ >= settleUs_)
            {
                setRelay(false, nowUs);
            }
            break;

        case State::WaitingForOn:
        case State::WaitingForOff:
            if (nowUs - commandAtUs_ >= timeoutUs_)
            {
                digitalWrite(controlPin_, LOW);
                state_ = State::Failed;
            }
            break;

        case State::Complete:
        case State::Failed:
            break;
    }
}

bool RelayTester::takeMeasurement(RelayMeasurement& measurement)
{
    if (!measurementReady_)
    {
        return false;
    }

    measurement = measurement_;
    measurementReady_ = false;
    return true;
}

bool RelayTester::complete() const
{
    return state_ == State::Complete;
}

bool RelayTester::failed() const
{
    return state_ == State::Failed;
}

uint32_t RelayTester::averageOnUs() const
{
    return onCount_ == 0 ? 0 : static_cast<uint32_t>(onTotalUs_ / onCount_);
}

uint32_t RelayTester::averageOffUs() const
{
    return offCount_ == 0 ? 0 : static_cast<uint32_t>(offTotalUs_ / offCount_);
}

bool RelayTester::updateContact(
    uint32_t nowUs,
    bool& closed,
    uint32_t& changedAtUs
)
{
    noInterrupts();
    const bool changed = contactChanged_;
    const uint32_t edgeAtUs = contactEdgeAtUs_;
    contactChanged_ = false;
    interrupts();

    if (changed)
    {
        candidateClosed_ = digitalRead(contactPin_) == LOW;
        candidateAtUs_ = edgeAtUs;
        candidatePending_ = true;
    }

    if (!candidatePending_ || nowUs - candidateAtUs_ < debounceUs_)
    {
        return false;
    }

    const bool currentClosed = digitalRead(contactPin_) == LOW;
    if (currentClosed != candidateClosed_)
    {
        candidatePending_ = false;
        return false;
    }

    candidatePending_ = false;
    if (currentClosed == stableClosed_)
    {
        return false;
    }

    stableClosed_ = currentClosed;
    closed = currentClosed;
    changedAtUs = candidateAtUs_;
    return true;
}

void RelayTester::processContact(
    bool closed,
    uint32_t changedAtUs,
    uint32_t nowUs
)
{
    if (state_ == State::WaitingForOn && closed)
    {
        if (!warmup_)
        {
            storeMeasurement(
                RelayTransition::TurnOn,
                changedAtUs - commandAtUs_
            );
        }

        state_ = State::HoldingOn;
        stateStartedAtUs_ = nowUs;
    }
    else if (state_ == State::WaitingForOff && !closed)
    {
        if (warmup_)
        {
            warmup_ = false;
        }
        else
        {
            storeMeasurement(
                RelayTransition::TurnOff,
                changedAtUs - commandAtUs_
            );
        }

        if (onCount_ >= measurementCount_ && offCount_ >= measurementCount_)
        {
            state_ = State::Complete;
        }
        else
        {
            state_ = State::HoldingOff;
            stateStartedAtUs_ = nowUs;
        }
    }
}

void RelayTester::setRelay(bool enabled, uint32_t nowUs)
{
    commandAtUs_ = nowUs;
    state_ = enabled ? State::WaitingForOn : State::WaitingForOff;
    digitalWrite(controlPin_, enabled ? HIGH : LOW);
}

void RelayTester::storeMeasurement(
    RelayTransition transition,
    uint32_t durationUs
)
{
    uint8_t number = 0;

    if (transition == RelayTransition::TurnOn)
    {
        onTotalUs_ += durationUs;
        number = ++onCount_;
    }
    else
    {
        offTotalUs_ += durationUs;
        number = ++offCount_;
    }

    measurement_ = {transition, number, durationUs};
    measurementReady_ = true;
}
