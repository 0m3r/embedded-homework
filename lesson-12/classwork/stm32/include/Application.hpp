#pragma once

#include "Button.hpp"
#include "Clock.hpp"
#include "DebugLogger.hpp"
#include "Led.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);

private:
    static constexpr std::uint32_t DebounceMs = 100;

    Clock clock_;
    Led redLed_{GPIOC, GPIO_PIN_13, true};
    Led greenLed_{GPIOC, GPIO_PIN_15, false};
    Button redButton_{clock_, GPIO_PIN_5, DebounceMs};
    Button greenButton_{clock_, GPIO_PIN_6, DebounceMs};
    DebugLogger logger_;
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
