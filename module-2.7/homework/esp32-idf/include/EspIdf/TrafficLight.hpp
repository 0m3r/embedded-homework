#pragma once

#include <atomic>
#include <cstdint>

#include "EspIdf/Led.hpp"
#include "EspIdf/OneShotTimer.hpp"
#include "esp_err.h"

class TrafficLight {
public:
    TrafficLight(
        gpio_num_t redPin,
        gpio_num_t yellowPin,
        gpio_num_t greenPin);

    esp_err_t init();
    void update();
    void toggleSpecialMode();

private:
    enum class State {
        Green,
        GreenFlashingOn,
        GreenFlashingOff,
        Yellow,
        Red,
        RedYellow,
        FlashingYellowOn,
        FlashingYellowOff,
    };

    static void timerCallback(void* context);

    void next();
    void enter(State state);
    void setLights(bool red, bool yellow, bool green);
    const char* stateName() const;

    Led red_;
    Led yellow_;
    Led green_;
    OneShotTimer timer_{"traffic_light"};
    State state_ = State::Red;
    std::uint32_t greenFlashStep_ = 0;
    bool specialMode_ = false;
    std::atomic_bool timeout_{false};
};
