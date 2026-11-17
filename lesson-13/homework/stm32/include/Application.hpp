#pragma once

#include "Config.hpp"
#include "DebugLogger.hpp"
#include "FanController.hpp"
#include "PeriodicTimer.hpp"

class Application {
public:
    void init();
    void update();

private:
    static void onTimer(void* context);

    DebugLogger logger_;
    PeriodicTimer timer_;
    FanController fan_{
        Config::FanPort,
        Config::FanPin,
        Config::FanActiveHigh,
        Config::CyclePeriodSeconds,
        Config::RunDurationSeconds};
};

extern "C" void appInit();
extern "C" void appUpdate();
