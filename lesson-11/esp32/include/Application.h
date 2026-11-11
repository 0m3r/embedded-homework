#pragma once

#include "BlinkingLed.h"
#include "Button.h"
#include "Clock.h"
#include "Logger.h"

class Application
{
public:
    Application();
    void setup();
    void loop();

private:
    void setRunning(bool running);

    Clock clock_;
    Logger logger_;
    Button button_;
    BlinkingLed redLed_;
    BlinkingLed greenLed_;
    BlinkingLed blueLed_;
    bool running_ = true;
};
