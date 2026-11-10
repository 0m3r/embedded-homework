#pragma once

#include "Logger.h"
#include "Relay.h"
#include "RelayTester.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    void reportMeasurement(const RelayMeasurement& measurement);

    Logger logger_;
    Relay relay_;
    RelayTester relayTester_;
    bool summaryPrinted_ = false;
    bool errorPrinted_ = false;
};
