#pragma once

#include <atomic>

#include "EspIdf/Button.hpp"
#include "EspIdf/Config.hpp"
#include "EspIdf/Debouncer/TimerFsmDebouncer.hpp"
#include "EspIdf/TrafficLight.hpp"
#include "esp_err.h"

class Application {
public:
    esp_err_t init();

private:
    static void task(void* context);
    static void modeButtonPressed(void* context);
    void run();

    Button modeButton_{Config::ModeButtonPin, true, true};
    TimerFsmDebouncer modeButtonDebouncer_{
        modeButton_,
        Config::DebounceMs};
    TrafficLight trafficLight_{
        Config::RedLedPin,
        Config::YellowLedPin,
        Config::GreenLedPin};
    std::atomic_bool modeToggleRequested_{false};
};
