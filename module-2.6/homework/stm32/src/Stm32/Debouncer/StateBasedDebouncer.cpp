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
    case State::Idle:
        if (input.interruptCount > 0) {
            state_ = State::Pressed;
            transitionStartedAtMs_ = input.nowMs;
        }
        break;

    case State::Pressed:
        if (input.interruptCount > 0) {
            transitionStartedAtMs_ = input.nowMs;
        } else if (input.nowMs - transitionStartedAtMs_ >= debounceMs_) {
            state_ = input.pressed ? State::Held : State::Idle;
            if (input.pressed) {
                logger_.press(source_, "State FSM", ++count_);
            }
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

std::uint32_t StateBasedDebouncer::count() const
{
    return count_;
}
