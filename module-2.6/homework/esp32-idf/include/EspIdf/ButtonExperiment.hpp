#pragma once

#include <cstdint>

#include "EspIdf/Button.hpp"
#include "EspIdf/Debouncer/NoDebouncer.hpp"
#include "EspIdf/Debouncer/PollingFsmDebouncer.hpp"
#include "EspIdf/Debouncer/TimeBasedDebouncer.hpp"
#include "EspIdf/Debouncer/TimerFsmDebouncer.hpp"
#include "EspIdf/Led.hpp"
#include "esp_err.h"

class ButtonExperiment {
public:
    ButtonExperiment(
        const char* name,
        gpio_num_t buttonPin,
        bool internalPullDown,
        gpio_num_t ledPin,
        std::uint32_t debounceMs);

    esp_err_t init();
    void update(std::uint64_t nowMs);

private:
    static void timerFsmPressed(void* context);
    void onTimerFsmPressed();
    void logEdge(const Button::Edge& edge) const;
    void log(const char* method, std::uint32_t count) const;

    const char* const name_;
    Button button_;
    Led led_;
    NoDebouncer noDebouncer_;
    TimeBasedDebouncer timeDebouncer_;
    TimerFsmDebouncer timerFsm_;
    PollingFsmDebouncer pollingFsm_;
};
