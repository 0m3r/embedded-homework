#pragma once

#include "Config.hpp"
#include "FanController.hpp"
#include "PeriodicTimer.hpp"

class Application {
public:
    void setup();
    void loop();

private:
    static void ARDUINO_ISR_ATTR onTimer(void* context);

    PeriodicTimer timer_;
    FanController fan_{
        Config::FanPin,
        Config::FanActiveHigh,
        Config::RunDurationMs,
        Config::IdleDurationMs};
};
