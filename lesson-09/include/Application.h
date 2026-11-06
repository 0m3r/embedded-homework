#pragma once

#include "Button.h"
#include "Led.h"
#include "LedController.h"
#include "LoopProfiler.h"

class Application
{
public:
    Application();

    void setup();
    void loop();

private:
    Led led_;
    Button button_;
    LedController ledController_;
    LoopProfiler loopProfiler_;
};
