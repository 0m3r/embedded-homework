#pragma once

#include "Button.hpp"
#include "Config.hpp"
#include "Led.hpp"
#include "PeriodicTimer.hpp"

class Application {
public:
    void setup();
    void loop();

private:
    Button button_{Config::ButtonPin};
    Led led_{Config::LedPin};
    PeriodicTimer blinkTimer_;
    std::uint32_t lastPressMs_ = 0;
    bool blinking_ = false;
};
