#include "Debouncer/PollingDebouncer.hpp"

PollingDebouncer::PollingDebouncer(const DebugLogger& logger,
                                   const char* source,
                                   std::uint32_t pollIntervalMs,
                                   std::uint32_t debounceMs)
    : logger_(logger),
      source_(source),
      pollIntervalMs_(pollIntervalMs),
      debounceMs_(debounceMs)
{
}

void PollingDebouncer::debounce(const DebounceInput& input)
{
    if (input.nowMs - lastPollAtMs_ < pollIntervalMs_) {
        return;
    }
    lastPollAtMs_ = input.nowMs;

    switch (state_) {
    case State::Released:
        if (input.pressed) {
            state_ = State::DebouncingPress;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::DebouncingPress:
        if (!input.pressed) {
            state_ = State::Released;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = State::Pressed;
            logger_.press(source_, "Polling FSM", ++count_);
        }
        break;

    case State::Pressed:
        if (!input.pressed) {
            state_ = State::DebouncingRelease;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::DebouncingRelease:
        if (input.pressed) {
            state_ = State::Pressed;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = State::Released;
        }
        break;
    }
}
