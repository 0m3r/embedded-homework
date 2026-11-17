#include "PeriodicTimer.hpp"

#include <limits>

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(std::uint32_t intervalMs)
{
    if (intervalMs == 0) {
        return false;
    }

    timer_ = timerBegin(CounterFrequencyHz);
    if (timer_ == nullptr) {
        return false;
    }

    instance_ = this;
    timerStop(timer_);
    timerAttachInterrupt(timer_, handleInterrupt);

    const std::uint64_t alarmTicks =
        static_cast<std::uint64_t>(intervalMs) *
        CounterFrequencyHz / 1'000;

    timerAlarm(timer_, alarmTicks, true, 0);
    timerStop(timer_);
    timerWrite(timer_, 0);
    return true;
}

bool PeriodicTimer::start()
{
    if (timer_ == nullptr) {
        return false;
    }

    portENTER_CRITICAL(&mux_);
    pendingPeriods_ = 0;
    portEXIT_CRITICAL(&mux_);

    timerWrite(timer_, 0);
    timerStart(timer_);
    return true;
}

void PeriodicTimer::stop()
{
    if (timer_ == nullptr) {
        return;
    }

    timerStop(timer_);
    timerWrite(timer_, 0);

    portENTER_CRITICAL(&mux_);
    pendingPeriods_ = 0;
    portEXIT_CRITICAL(&mux_);
}

std::uint32_t PeriodicTimer::getPeriods()
{
    portENTER_CRITICAL(&mux_);
    const auto periods = pendingPeriods_;
    pendingPeriods_ = 0;
    portEXIT_CRITICAL(&mux_);
    return periods;
}

void ARDUINO_ISR_ATTR PeriodicTimer::handleInterrupt()
{
    if (instance_ == nullptr) {
        return;
    }

    auto& timer = *instance_;
    portENTER_CRITICAL_ISR(&timer.mux_);
    if (timer.pendingPeriods_ < std::numeric_limits<std::uint32_t>::max()) {
        timer.pendingPeriods_ = timer.pendingPeriods_ + 1U;
    }
    portEXIT_CRITICAL_ISR(&timer.mux_);
}
