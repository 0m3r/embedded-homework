#include "Application.hpp"

#include <Arduino.h>

void Application::setup()
{
    Serial.begin(Config::SerialBaud);
    enableLoopWDT();

    if (!fan_.begin()) {
        fail("Fan controller initialization failed");
    }

    Serial.printf("Fan scheduler: GPIO%u, period=%lus, run=%lus\n",
                  Config::FanPin,
                  static_cast<unsigned long>(Config::CyclePeriodMs / 1'000),
                  static_cast<unsigned long>(Config::RunDurationMs / 1'000));
}

void Application::loop()
{
    feedLoopWDT();

    switch (fan_.takeEvent()) {
    case FanController::Event::Started:
        Serial.println("Fan ON");
        break;

    case FanController::Event::Stopped:
        Serial.println("Fan OFF");
        break;

    case FanController::Event::None:
        break;
    }
}

void Application::fail(const char* message)
{
    Serial.println(message);
    while (true) {
    }
}
