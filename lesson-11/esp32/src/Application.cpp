#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      button_(clock_, Config::ButtonPin, Config::ButtonDebounceMs),
      redLed_(clock_, Config::RedLedPin, Config::RedIntervalMs),
      greenLed_(clock_, Config::GreenLedPin, Config::GreenIntervalMs),
      blueLed_(clock_, Config::BlueLedPin, Config::BlueIntervalMs)
{
}

void Application::setup()
{
    clock_.update();
    logger_.begin();
    button_.init();
    redLed_.init();
    greenLed_.init();
    blueLed_.init();
    logger_.log("Superloop started: LEDs running");
}

void Application::loop()
{
    clock_.update();

    if (button_.update())
    {
        setRunning(!running_);
    }

    redLed_.update();
    greenLed_.update();
    blueLed_.update();
}

void Application::setRunning(bool running)
{
    running_ = running;

    if (running_)
    {
        redLed_.on();
        greenLed_.on();
        blueLed_.on();
    }
    else
    {
        redLed_.off();
        greenLed_.off();
        blueLed_.off();
    }

    logger_.log(running_ ? "LEDs running" : "LEDs stopped");
}
