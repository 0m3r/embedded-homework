#include "RelayTester.h"

void RelayTester::init(uint32_t nowUs)
{
    relay_.init();
    stateStartedAtUs_ = nowUs;
}

void RelayTester::update(uint32_t nowUs)
{
    RelayContactEvent event{};
    if (relay_.update(nowUs, event))
    {
        processContact(event, nowUs);
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
                relay_.setEnabled(false);
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

void RelayTester::processContact(
    const RelayContactEvent& event,
    uint32_t nowUs
)
{
    if (state_ == State::WaitingForOn && event.closed)
    {
        if (!warmup_)
        {
            storeMeasurement(
                RelayTransition::TurnOn,
                event.changedAtUs - commandAtUs_
            );
        }

        state_ = State::HoldingOn;
        stateStartedAtUs_ = nowUs;
    }
    else if (state_ == State::WaitingForOff && !event.closed)
    {
        if (warmup_)
        {
            warmup_ = false;
        }
        else
        {
            storeMeasurement(
                RelayTransition::TurnOff,
                event.changedAtUs - commandAtUs_
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
    relay_.setEnabled(enabled);
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
