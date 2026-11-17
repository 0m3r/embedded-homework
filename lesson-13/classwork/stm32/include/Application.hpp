#pragma once

#include "Button.hpp"
#include "Clock.hpp"
#include "DebugLogger.hpp"
#include "Led.hpp"
#include "PeriodicTimer.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);

private:
    static constexpr std::uint32_t DebounceMs = 100;

    Clock clock_;
    Led greenLed_{GPIOC, GPIO_PIN_13, true};
    Led redLed_{GPIOC, GPIO_PIN_15, false};
    Button greenButton_{clock_, GPIO_PIN_5, DebounceMs};
    Button redButton_{clock_, GPIO_PIN_6, DebounceMs};
    PeriodicTimer blinkTimer_;
    DebugLogger logger_;
    bool redBlinking_ = false;
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
