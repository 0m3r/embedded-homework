#include "Application.hpp"

#include <Arduino.h>

void Application::setup()
{
    Serial.begin(Config::SerialBaud);
    fan_.begin();

    if (!timer_.init(Config::TimerTickMs, onTimer, this) ||
        !timer_.start()) {
        Serial.println("Hardware timer initialization failed");
        while (true) {
        }
    }

    Serial.printf("Fan scheduler: GPIO%u, period=%lus, run=%lus\n",
                  Config::FanPin,
                  static_cast<unsigned long>(Config::CyclePeriodSeconds),
                  static_cast<unsigned long>(Config::RunDurationSeconds));
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
    static_cast<Application*>(context)->fan_.update();
}
