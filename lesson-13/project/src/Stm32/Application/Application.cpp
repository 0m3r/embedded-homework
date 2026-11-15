#include "Stm32/Application/Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    clock_.update();
    softwareExperiment_.init();
    hardwareRcExperiment_.init();
    logger_.log("STM32 debounce comparison ready\r\n");
}

void Application::update()
{
    clock_.update();

    softwareExperiment_.update(clock_.now());
    hardwareRcExperiment_.update(clock_.now());

    logger_.update();
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    softwareExperiment_.onInterrupt(pin);
    hardwareRcExperiment_.onInterrupt(pin);
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
