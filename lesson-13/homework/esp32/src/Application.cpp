#include "Application.hpp"

#include <Arduino.h>

void Application::setup()
{
    Serial.begin(Config::SerialBaud);
    fan_.begin();

    if (!timer_.init(onTimer, this) ||
        !timer_.startOnce(Config::CyclePeriodMs)) {
        Serial.println("Hardware timer initialization failed");
        while (true) {
        }
    }

    Serial.printf("Fan scheduler: GPIO%u, period=%lus, run=%lus\n",
                  Config::FanPin,
                   static_cast<unsigned long>(Config::CyclePeriodMs / 1'000),
                   static_cast<unsigned long>(Config::RunDurationMs / 1'000));
}

void Application::loop()
{
    const auto events = fan_.takeEvents();

    for (std::uint32_t i = 0; i < events.starts; ++i) {
        Serial.println("Fan ON");
    }

    for (std::uint32_t i = 0; i < events.stops; ++i) {
        Serial.println("Fan OFF");
    }
}

void ARDUINO_ISR_ATTR Application::onTimer(void* context)
{
    auto& application = *static_cast<Application*>(context);
    const auto nextIntervalMs = application.fan_.onTimeout();
    application.timer_.startOnce(nextIntervalMs);
}
