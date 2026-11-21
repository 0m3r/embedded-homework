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
    case State::Idle:
        if (input.pressed) {
            state_ = State::Pressed;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::Pressed:
        if (!input.pressed) {
            state_ = State::Idle;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = State::Held;
            logger_.press(source_, "Polling FSM", ++count_);
        }
        break;

    case State::Held:
        if (!input.pressed) {
            state_ = State::Release;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::Release:
        if (input.pressed) {
            state_ = State::Held;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = State::Idle;
        }
        break;
    }
}
