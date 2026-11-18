#include "OneShotTimer.hpp"

bool OneShotTimer::init(Callback callback, void* context)
{
    if (callback == nullptr) {
        return false;
    }

    timer_ = timerBegin(FrequencyHz);
    if (timer_ == nullptr) {
        return false;
    }

    timerStop(timer_);
    timerAttachInterruptArg(timer_, callback, context);
    timerWrite(timer_, 0);
    timerStart(timer_);
    return true;
}

bool ARDUINO_ISR_ATTR OneShotTimer::start(std::uint32_t intervalMs)
{
    if (timer_ == nullptr || intervalMs == 0) {
        return false;
    }

    const std::uint64_t intervalTicks =
        static_cast<std::uint64_t>(intervalMs) * FrequencyHz / 1'000;

    timerAlarm(timer_, timerRead(timer_) + intervalTicks, false, 0);
    return true;
}
