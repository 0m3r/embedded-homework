#pragma once

#include "Config.hpp"
#include "DebugLogger.hpp"
#include "FanController.hpp"

class Application {
public:
    void init();
    void update();

private:
    [[noreturn]] void fail(const char* message);

    IWDG_HandleTypeDef watchdog_{};
    DebugLogger logger_;
    FanController fan_{
        Config::FanPort,
        Config::FanPin,
        Config::FanActiveHigh,
        Config::RunDurationMs,
        Config::IdleDurationMs};
};

extern "C" void appInit();
extern "C" void appUpdate();
