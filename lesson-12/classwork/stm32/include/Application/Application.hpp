#pragma once

#include "Application/ButtonExperiment.hpp"
#include "Hardware/Clock.hpp"
#include "Hardware/DebugLogger.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);

private:
    static constexpr std::uint32_t DebounceMs = 100;
    static constexpr std::uint32_t PollIntervalMs = 5;

    Clock clock_;
    DebugLogger logger_;
    ButtonExperiment softwareExperiment_{
        logger_, "PA5 software", GPIOA, GPIO_PIN_5, true,
        GPIOC, GPIO_PIN_13, false, DebounceMs, PollIntervalMs};
    ButtonExperiment hardwareRcExperiment_{
        logger_, "PB6 hardware RC", GPIOB, GPIO_PIN_6, false,
        GPIOC, GPIO_PIN_15, false, DebounceMs, PollIntervalMs};
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
