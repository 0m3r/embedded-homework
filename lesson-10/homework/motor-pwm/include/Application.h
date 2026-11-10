#pragma once

#include <stdint.h>

#include "Clock.h"
#include "Logger.h"
#include "Potentiometer.h"
#include "SoftwarePwm.h"
#include "UartExperiment.h"
#include "Timer.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    Clock clock_;
    Logger logger_;
    Potentiometer potentiometer_;
    SoftwarePwm motorPwm_;
    UartExperiment uartExperiment_;
    Timer printTimer_;
};
