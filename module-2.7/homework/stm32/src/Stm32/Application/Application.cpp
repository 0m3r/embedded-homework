#include "Application/Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    modeButton_.init();
    modeButtonDebouncer_.init(modeButtonPressed, this);
    trafficLight_.init();
    logger_.log("STM32 traffic light ready\r\n");
}

void Application::update()
{
    if (modeButton_.takeInterrupts() > 0) {
        modeButtonDebouncer_.onEdge(modeButton_.isPressed());
    }

    modeButtonDebouncer_.update();
    trafficLight_.update();
    logger_.update();
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    modeButton_.onInterrupt(pin);
}

void Application::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    modeButtonDebouncer_.onTimerInterrupt(timer);
    trafficLight_.onTimerInterrupt(timer);
}

void Application::modeButtonPressed(void* context)
{
    auto* application = static_cast<Application*>(context);
    application->trafficLight_.toggleSpecialMode();
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
