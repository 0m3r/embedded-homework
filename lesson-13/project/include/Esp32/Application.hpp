#pragma once

#include "Debouncer/DebounceExperiment.hpp"
#include "Esp32/Button.hpp"
#include "Esp32/Config.hpp"
#include "Esp32/Logger.hpp"

class Application {
public:
    Application();
    void setup();
    void loop();

private:
    Logger logger_;
    Button button_{Config::ButtonPin};
    DebounceExperiment experiment_{
        logger_, Config::DebounceMs, Config::PollIntervalMs};
};
