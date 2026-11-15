#include "Debouncer/StateBasedDebouncer.hpp"

StateBasedDebouncer::StateBasedDebouncer(const Logger& logger,
                                         std::uint32_t debounceMs)
    : logger_(logger), debounceMs_(debounceMs)
{
}

void StateBasedDebouncer::debounce(const DebounceInput& input)
{
    if (input.interruptCount > 0 && !pressLatched_) {
        eventPending_ = true;
        eventStartedAtMs_ = input.nowMs;
    }

    if (eventPending_ && input.nowMs - eventStartedAtMs_ >= debounceMs_) {
        eventPending_ = false;
        if (input.pressed && !pressLatched_) {
            pressLatched_ = true;
            releasePending_ = false;
            logger_.press("State-based", ++count_);
        }
    }

    if (!pressLatched_) {
        return;
    }

    if (input.pressed) {
        releasePending_ = false;
        return;
    }

    if (!releasePending_) {
        releasePending_ = true;
        releaseStartedAtMs_ = input.nowMs;
    } else if (input.nowMs - releaseStartedAtMs_ >= debounceMs_) {
        pressLatched_ = false;
        releasePending_ = false;
    }
}
