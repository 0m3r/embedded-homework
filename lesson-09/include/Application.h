#pragma once

#include "Button.h"
#include "LedController.h"
#include "Logger.h"
#include "LoopProfiler.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    Logger logger_;
    Button button_;
    LedController ledController_;
    LoopProfiler loopProfiler_;
};
