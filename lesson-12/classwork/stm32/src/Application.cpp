#include "Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    clock_.update();
    redLed_.off();
    greenLed_.off();
    redButton_.init();
    greenButton_.init();
}

void Application::update()
{
    clock_.update();

    if (redButton_.wasPressed()) {
        redLed_.toggle();
        logger_.log("Red button pressed\r\n");
    }

    if (greenButton_.wasPressed()) {
        greenLed_.toggle();
        logger_.log("Green button pressed\r\n");
    }
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    redButton_.onInterrupt(pin);
    greenButton_.onInterrupt(pin);
}

extern "C" void appInit()
{
    application.init();
}

extern "C" void appUpdate()
{
    application.update();
}

extern "C" void appButtonInterrupt(std::uint16_t pin)
{
    application.onButtonInterrupt(pin);
}
