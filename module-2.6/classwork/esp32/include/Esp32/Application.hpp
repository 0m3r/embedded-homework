#pragma once

#include "Esp32/Button.hpp"
#include "Esp32/Config.hpp"
#include "Esp32/Led.hpp"

class Application {
public:
    void setup();
    void loop();

private:
    [[noreturn]] void fail(const char* message);

    Button button_{Config::ButtonPin, Config::DebounceMs};
    Led led_{Config::LedPin};
};
