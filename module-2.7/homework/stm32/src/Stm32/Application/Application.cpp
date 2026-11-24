#include "Application/Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    trafficLight_.init();
    logger_.log("STM32 traffic light ready\r\n");
}

void Application::update()
{
    trafficLight_.update();
    logger_.update();
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    trafficLight_.onButtonInterrupt(pin);
}

void Application::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    trafficLight_.onTimerInterrupt(timer);
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

extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer)
{
    application.onTimerInterrupt(timer);
}
