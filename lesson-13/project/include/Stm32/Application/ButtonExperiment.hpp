#pragma once

#include <cstdint>

#include "Debouncer/DebounceExperiment.hpp"
#include "Stm32/Hardware/Button.hpp"
#include "Stm32/Hardware/DebugLogger.hpp"
#include "Stm32/Hardware/Led.hpp"

class ButtonExperiment : public IReporter {
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
    void press(const char* method, std::uint32_t count) const override;

    DebugLogger& logger_;
    const char* const source_;
    Button button_;
    Led led_;
    DebounceExperiment experiment_;
    std::uint32_t ledCount_ = 0;
};
