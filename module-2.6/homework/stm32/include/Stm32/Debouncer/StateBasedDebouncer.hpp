#pragma once

#include <cstdint>

#include "Hardware/DebugLogger.hpp"
#include "Debouncer/IDebouncer.hpp"

class StateBasedDebouncer : public IDebouncer {
public:
    StateBasedDebouncer(const DebugLogger& logger,
                        const char* source,
                        std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;
    std::uint32_t count() const;

private:
    enum class State {
        Released,
        DebouncingPress,
        Pressed,
        DebouncingRelease,
    };

    const DebugLogger& logger_;
    const char* const source_;
    const std::uint32_t debounceMs_;
    std::uint32_t transitionStartedAtMs_ = 0;
    std::uint32_t count_ = 0;
    State state_ = State::Released;
};
