#include "Esp32/Application.hpp"

#include <Arduino.h>

Application::Application() = default;

void Application::setup()
{
    logger_.begin(Config::SerialBaud);
    button_.begin();
    logger_.started(Config::ButtonPin);
}

void Application::loop()
{
    const DebounceInput input{
        button_.takeInterrupts(),
        button_.isPressed(),
        millis(),
    };

    experiment_.update(input);
}
