#include "Application/Application.hpp"

namespace {
Application application;
}

extern "C" void Error_Handler();

void Application::init()
{
    if (!events_.registerHandler(trafficLight_)) {
        Error_Handler();
    }

    trafficLight_.init();
    logger_.log("STM32 traffic light ready\r\n");
}

void Application::update()
{
    events_.dispatchPending();
    logger_.update();
}

void Application::waitForEvent()
{
    eventWaiter_.wait(events_);
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    eventProducer_.onButtonInterrupt(pin);
}

void Application::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    eventProducer_.onTimerInterrupt(timer);
}

extern "C" void appInit()
{
    application.init();
}

extern "C" void appUpdate()
{
    application.update();
}

extern "C" void appWaitForEvent()
{
    application.waitForEvent();
}

extern "C" void appButtonInterrupt(std::uint16_t pin)
{
    application.onButtonInterrupt(pin);
}

extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer)
{
    application.onTimerInterrupt(timer);
}
