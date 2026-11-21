#pragma once

#include "EspIdf/Button.hpp"
#include "EspIdf/Config.hpp"
#include "EspIdf/Debouncer/NoDebouncer.hpp"
#include "EspIdf/Debouncer/PollingFsmDebouncer.hpp"
#include "EspIdf/Debouncer/TimeBasedDebouncer.hpp"
#include "EspIdf/Debouncer/TimerFsmDebouncer.hpp"
#include "EspIdf/Led.hpp"
#include "esp_err.h"

class Application {
public:
    esp_err_t init();

private:
    static void task(void* context);
    static void buttonPressed(void* context);
    void run();
    void onButtonPressed();

    Button button_{Config::ButtonPin};
    Led led_{Config::LedPin};
    NoDebouncer noDebouncer_;
    TimeBasedDebouncer timeDebouncer_{Config::DebounceMs};
    TimerFsmDebouncer timerFsm_{button_, Config::DebounceMs};
    PollingFsmDebouncer pollingFsm_{Config::DebounceMs};
};
