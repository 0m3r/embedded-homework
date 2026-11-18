#pragma once

#include "Config.hpp"
#include "FanController.hpp"

class Application {
public:
    void setup();
    void loop();

private:
    [[noreturn]] void fail(const char* message);

    FanController fan_{
        Config::FanPin,
        Config::FanActiveHigh,
        Config::RunDurationMs,
        Config::IdleDurationMs};
};
