#pragma once

#include "IntervalTimer.hpp"
#include "Led.hpp"

class Application {
public:
    void init();
    void update();

private:
    Led led1_{GPIOC, GPIO_PIN_13, true};
    Led led2_{GPIOB, GPIO_PIN_0, false};
    Led led3_{GPIOB, GPIO_PIN_1, false};

    IntervalTimer timer1_{250};
    IntervalTimer timer2_{500};
    IntervalTimer timer3_{1000};
};
