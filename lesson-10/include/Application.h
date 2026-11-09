#pragma once

#include <stdint.h>

#include "Logger.h"
#include "Potentiometer.h"
#include "RelayTester.h"
#include "SoftwarePwm.h"
#include "UartExperiment.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    void reportRelayMeasurement(const RelayMeasurement& measurement);

    Logger logger_;
    RelayTester relayTester_;
    Potentiometer potentiometer_;
    SoftwarePwm motorPwm_;
    UartExperiment uartExperiment_;

    uint32_t lastPrintAtMs_ = 0;
    bool relaySummaryPrinted_ = false;
    bool relayErrorPrinted_ = false;
};
