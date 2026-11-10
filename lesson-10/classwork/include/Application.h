#pragma once

#include <stdint.h>

#include "Ldr.h"
#include "Logger.h"
#include "MotorController.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    Logger logger_;
    Ldr ldr_;
    MotorController motorController_;
    uint32_t lastPrintAt_ = 0;
};
