#include "Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    clock_.update();
    greenLed_.off();
    redLed_.off();
    greenButton_.init();
    redButton_.init();

    if (!blinkTimer_.init(500)) {
        logger_.log("TIM2 initialization failed\r\n");
    }
}

void Application::update()
{
    clock_.update();

    if (greenButton_.wasPressed()) {
        greenLed_.toggle();
        logger_.log("Green button pressed\r\n");
    }

    if (redButton_.wasPressed()) {
        redBlinking_ = !redBlinking_;

        if (redBlinking_) {
            blinkTimer_.start();
            logger_.log("Red LED blinking started\r\n");
        } else {
            blinkTimer_.stop();
            redLed_.off();
            logger_.log("Red LED blinking stopped\r\n");
        }
    }

    const auto elapsedPeriods = blinkTimer_.getPeriods();
    if (redBlinking_ && (elapsedPeriods % 2U) != 0U) {
        redLed_.toggle();
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
