#include "Stm32/Application.hpp"

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

    led1_.init();
    led2_.init();

    if (!button1_.init() || !button2_.init()) {
        fail("Button initialization failed\r\n");
    }

    logger_.log("Debounce ready: PB0->PC13, PB6->PC14\r\n");
}

void Application::update()
{
    HAL_IWDG_Refresh(&watchdog_);

    if (button1_.takePress()) {
        led1_.toggle();
        logger_.log(led1_.isOn() ? "PC13 ON\r\n" : "PC13 OFF\r\n");
    }

    if (button2_.takePress()) {
        led2_.toggle();
        logger_.log(led2_.isOn() ? "PC14 ON\r\n" : "PC14 OFF\r\n");
    }
}

void Application::onButtonInterrupt(std::uint16_t pin)
{
    button1_.onInterrupt(pin);
    button2_.onInterrupt(pin);
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

extern "C" void appButtonInterrupt(std::uint16_t pin)
{
    application.onButtonInterrupt(pin);
}
