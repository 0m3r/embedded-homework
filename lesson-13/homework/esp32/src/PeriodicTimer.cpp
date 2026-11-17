#include "PeriodicTimer.hpp"

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(Handler handler, void* context)
{
    if (handler == nullptr) {
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
    timerWrite(timer_, 0);
    return true;
}

bool ARDUINO_ISR_ATTR PeriodicTimer::startOnce(std::uint32_t intervalMs)
{
    if (timer_ == nullptr || intervalMs == 0) {
        return false;
    }

    const std::uint64_t alarmTicks =
        static_cast<std::uint64_t>(intervalMs) *
        CounterFrequencyHz / 1'000;

    timerAlarm(timer_, alarmTicks, false, 0);
    timerWrite(timer_, 0);

    if (!started_) {
        timerStart(timer_);
        started_ = true;
    }
    return true;
}

void PeriodicTimer::stop()
{
    if (timer_ == nullptr) {
        return;
    }

    timerStop(timer_);
    timerWrite(timer_, 0);
    started_ = false;
}

void ARDUINO_ISR_ATTR PeriodicTimer::handleInterrupt()
{
    if (instance_ != nullptr && instance_->handler_ != nullptr) {
        instance_->handler_(instance_->context_);
    }
}
