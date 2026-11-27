#include "Core/Output/LedFlasher.hpp"

LedFlasher::LedFlasher(IOneShotTimer& timer)
    : timer_(timer)
{
}

void LedFlasher::start(
    ILed& led,
    std::uint32_t halfPeriodMs,
    std::uint32_t halfPeriods)
{
    led_ = &led;
    halfPeriodMs_ = halfPeriodMs;
    remainingHalfPeriods_ = halfPeriods;
    continuous_ = halfPeriods == Continuous;

    led_->on();
    timer_.start(halfPeriodMs_);
}

bool LedFlasher::onTimeout()
{
    if (led_ == nullptr) {
        return true;
    }

    if (!continuous_) {
        --remainingHalfPeriods_;
        if (remainingHalfPeriods_ == 0) {
            stop();
            return true;
        }
    }

    led_->toggle();
    timer_.start(halfPeriodMs_);
    return false;
}

void LedFlasher::stop()
{
    led_ = nullptr;
    halfPeriodMs_ = 0;
    remainingHalfPeriods_ = 0;
    continuous_ = false;
}
