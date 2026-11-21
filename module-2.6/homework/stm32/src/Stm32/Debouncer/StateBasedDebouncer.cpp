#include "Debouncer/StateBasedDebouncer.hpp"

StateBasedDebouncer::StateBasedDebouncer(const DebugLogger& logger,
                                         const char* source,
                                         std::uint32_t debounceMs)
    : logger_(logger), source_(source), debounceMs_(debounceMs)
{
}

void StateBasedDebouncer::debounce(const DebounceInput& input)
{
    switch (state_) {
    case State::Released:
        if (input.interruptCount > 0) {
            state_ = State::DebouncingPress;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::DebouncingPress:
        if (input.interruptCount > 0) {
            transitionStartedAtMs_ = input.nowMs;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = input.pressed ? State::Pressed : State::Released;
            if (input.pressed) {
                logger_.press(source_, "State FSM", ++count_);
            }
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

std::uint32_t StateBasedDebouncer::count() const
{
    return count_;
}
