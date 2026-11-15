#pragma once

#include <cstdint>

#include "Hardware/Button.hpp"
#include "Hardware/DebugLogger.hpp"
#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/NoDebounceCounter.hpp"
#include "Debouncer/PollingDebouncer.hpp"
#include "Debouncer/StateBasedDebouncer.hpp"
#include "Debouncer/TimeBasedDebouncer.hpp"
#include "Hardware/Led.hpp"

class ButtonExperiment {
public:
    ButtonExperiment(DebugLogger& logger,
                     const char* source,
                     GPIO_TypeDef* buttonPort,
                     std::uint16_t buttonPin,
                     bool buttonActiveHigh,
                     GPIO_TypeDef* ledPort,
                     std::uint16_t ledPin,
                     bool ledActiveLow,
                     std::uint32_t debounceMs,
                     std::uint32_t pollIntervalMs);

    void init();
    void update(std::uint32_t nowMs);
    void onInterrupt(std::uint16_t pin);

private:
    Button button_;
    Led led_;
    NoDebounceCounter noDebounce_;
    TimeBasedDebouncer timeBased_;
    StateBasedDebouncer stateBased_;
    PollingDebouncer polling_;
    IDebouncer* debouncers_[4]{
        &noDebounce_, &timeBased_, &stateBased_, &polling_};
    std::uint32_t ledCount_ = 0;
};
