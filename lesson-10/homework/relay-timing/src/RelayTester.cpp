#include "RelayTester.h"

void RelayTester::init(uint32_t nowUs)
{
    relay_.init();
    contact_.init();
    stateStartedAtUs_ = nowUs;
}

bool RelayTester::tick(
    uint32_t nowUs,
    RelayMeasurement& measurement
)
{
    bool measured = false;

    if (contact_.tick(nowUs))
    {
        measured = processContact(nowUs, measurement);
    }

    switch (state_)
    {
        case State::SettlingBeforeOn:
        case State::HoldingOff:
            if (nowUs - stateStartedAtUs_ >= config_.settleUs)
            {
                setRelay(true, nowUs);
            }
            break;

        case State::HoldingOn:
            if (nowUs - stateStartedAtUs_ >= config_.settleUs)
            {
                setRelay(false, nowUs);
            }
            break;

        case State::WaitingForOn:
        case State::WaitingForOff:
            if (nowUs - commandAtUs_ >= config_.timeoutUs)
            {
                relay_.off();
                state_ = State::Failed;
            }
            break;

        case State::Complete:
        case State::Failed:
            break;
    }

    return measured;
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
    return statistics_.completedCycles == 0
        ? 0
        : static_cast<uint32_t>(
            statistics_.onTotalUs / statistics_.completedCycles
        );
}

uint32_t RelayTester::averageOffUs() const
{
    return statistics_.completedCycles == 0
        ? 0
        : static_cast<uint32_t>(
            statistics_.offTotalUs / statistics_.completedCycles
        );
}

bool RelayTester::processContact(
    uint32_t nowUs,
    RelayMeasurement& measurement
)
{
    bool measured = false;

    if (state_ == State::WaitingForOn && contact_.isClosed())
    {
        if (!warmup_)
        {
            recordMeasurement(
                RelayTransition::TurnOn,
                contact_.changedAtUs() - commandAtUs_,
                measurement
            );
            measured = true;
        }

        state_ = State::HoldingOn;
        stateStartedAtUs_ = nowUs;
    }
    else if (state_ == State::WaitingForOff && !contact_.isClosed())
    {
        if (warmup_)
        {
            warmup_ = false;
        }
        else
        {
            recordMeasurement(
                RelayTransition::TurnOff,
                contact_.changedAtUs() - commandAtUs_,
                measurement
            );
            measured = true;
        }

        if (statistics_.completedCycles >= config_.measurementCount)
        {
            state_ = State::Complete;
        }
        else
        {
            state_ = State::HoldingOff;
            stateStartedAtUs_ = nowUs;
        }
    }

    return measured;
}

void RelayTester::setRelay(bool enabled, uint32_t nowUs)
{
    commandAtUs_ = nowUs;
    state_ = enabled ? State::WaitingForOn : State::WaitingForOff;

    if (enabled)
    {
        relay_.on();
    }
    else
    {
        relay_.off();
    }
}

void RelayTester::recordMeasurement(
    RelayTransition transition,
    uint32_t durationUs,
    RelayMeasurement& measurement
)
{
    const uint8_t number = statistics_.completedCycles + 1;

    if (transition == RelayTransition::TurnOn)
    {
        statistics_.onTotalUs += durationUs;
    }
    else
    {
        statistics_.offTotalUs += durationUs;
        ++statistics_.completedCycles;
    }

    measurement = {transition, number, durationUs};
}
