#include "PeriodicTimer.hpp"

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(std::uint32_t intervalMs,
                         Handler handler,
                         void* context)
{
    if (intervalMs == 0 || handler == nullptr) {
        return false;
    }

    timer_ = timerBegin(CounterFrequencyHz);
    if (timer_ == nullptr) {
        return false;
    }

    handler_ = handler;
    context_ = context;
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
}

void ARDUINO_ISR_ATTR PeriodicTimer::handleInterrupt()
{
    if (instance_ != nullptr && instance_->handler_ != nullptr) {
        instance_->handler_(instance_->context_);
    }
}
