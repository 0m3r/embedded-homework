#pragma once

#include <stdint.h>

#include "Logger.h"
#include "Potentiometer.h"
#include "SoftwarePwm.h"
#include "UartExperiment.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    Logger logger_;
    Potentiometer potentiometer_;
    SoftwarePwm motorPwm_;
    UartExperiment uartExperiment_;
    uint32_t lastPrintAtMs_ = 0;
};
