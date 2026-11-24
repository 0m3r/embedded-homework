#include "Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    watchdog_.Instance = IWDG;
    watchdog_.Init.Prescaler = IWDG_PRESCALER_128;
    watchdog_.Init.Reload = 1'000;

    if (HAL_IWDG_Init(&watchdog_) != HAL_OK) {
        NVIC_SystemReset();
    }

    if (!fan_.begin()) {
        fail("Fan controller initialization failed\r\n");
    }

    logger_.log("Fan scheduler ready on PC15\r\n");
}

void Application::update()
{
    HAL_IWDG_Refresh(&watchdog_);

    switch (fan_.takeEvent()) {
    case FanController::Event::Started:
        logger_.log("Fan ON\r\n");
        break;

    case FanController::Event::Stopped:
        logger_.log("Fan OFF\r\n");
        break;

    case FanController::Event::TimerError:
        fail("Fan timer restart failed\r\n");
        break;

    case FanController::Event::None:
        break;
    }
}

void Application::fail(const char* message)
{
    logger_.log(message);
    while (true) {
    }
}

extern "C" void appInit()
{
    application.init();
}

extern "C" void appUpdate()
{
    application.update();
}
