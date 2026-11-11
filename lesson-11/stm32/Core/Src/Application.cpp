#include "Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    led1_.off();
    led2_.off();
    led3_.off();
}

void Application::update()
{
    const auto now = HAL_GetTick();

    if (timer1_.elapsed(now)) {
        led1_.toggle();
    }
    if (timer2_.elapsed(now)) {
        led2_.toggle();
    }
    if (timer3_.elapsed(now)) {
        led3_.toggle();
    }
}

extern "C" void app_init(void)
{
    application.init();
}

extern "C" void app_update(void)
{
    application.update();
}
