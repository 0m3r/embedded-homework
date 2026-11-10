#include "RelayTester.h"

void RelayTester::init(uint32_t nowUs)
{
    relay_.init();
    contact_.init();
    pauseAtUs_ = nowUs;
}

void RelayTester::restart(uint32_t nowUs)
{
    relay_.off();
    phase_ = Phase::On;
    waiting_ = false;
    warmup_ = true;
    cycle_ = 0;
    commandAtUs_ = 0;
    pauseAtUs_ = nowUs;
    onTotalUs_ = 0;
    offTotalUs_ = 0;
}

bool RelayTester::tick(
    uint32_t nowUs,
    RelayMeasurement& measurement
)
{
    const bool contactChanged = contact_.tick(nowUs);

    if (phase_ == Phase::Complete || phase_ == Phase::Failed)
    {
        return false;
    }

    if (waiting_)
    {
        const bool expectedClosed = phase_ == Phase::On;

        if (contactChanged && contact_.isClosed() == expectedClosed)
        {
            return finishTransition(nowUs, measurement);
        }

        if (nowUs - commandAtUs_ >= timeoutUs_)
        {
            relay_.off();
            phase_ = Phase::Failed;
        }

        return false;
    }

    if (nowUs - pauseAtUs_ >= settleUs_)
    {
        startTransition(nowUs);
    }

    return false;
}

bool RelayTester::complete() const
{
    return phase_ == Phase::Complete;
}

bool RelayTester::failed() const
{
    return phase_ == Phase::Failed;
}

uint32_t RelayTester::averageOnUs() const
{
    return cycle_ == 0
        ? 0
        : static_cast<uint32_t>(onTotalUs_ / cycle_);
}

uint32_t RelayTester::averageOffUs() const
{
    return cycle_ == 0
        ? 0
        : static_cast<uint32_t>(offTotalUs_ / cycle_);
}

void RelayTester::startTransition(uint32_t nowUs)
{
    commandAtUs_ = nowUs;
    waiting_ = true;

    if (phase_ == Phase::On)
    {
        relay_.on();
    }
    else
    {
        relay_.off();
    }
}

bool RelayTester::finishTransition(
    uint32_t nowUs,
    RelayMeasurement& measurement
)
{
    const uint32_t durationUs = contact_.changedAtUs() - commandAtUs_;
    const bool measured = !warmup_;

    if (measured)
    {
        measurement = {
            phase_ == Phase::On
                ? RelayTransition::On
                : RelayTransition::Off,
            static_cast<uint8_t>(cycle_ + 1),
            durationUs
        };

        if (phase_ == Phase::On)
        {
            onTotalUs_ += durationUs;
        }
        else
        {
            offTotalUs_ += durationUs;
            ++cycle_;
        }
    }

    if (phase_ == Phase::On)
    {
        phase_ = Phase::Off;
    }
    else if (warmup_)
    {
        warmup_ = false;
        phase_ = Phase::On;
    }
    else
    {
        phase_ = cycle_ >= measurementCount_
            ? Phase::Complete
            : Phase::On;
    }

    waiting_ = false;
    pauseAtUs_ = nowUs;
    return measured;
}
