#pragma once

#include <stdint.h>

#include "Logger.h"
#include "Relay.h"
#include "RelayContact.h"
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
    RelayContact contact_;
    RelayTester relayTester_;
    uint32_t completedAtMs_ = 0;
    bool summaryPrinted_ = false;
    bool errorPrinted_ = false;
};
