#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      ldr_(Config::LdrPin, Config::AdcIntervalMs),
      motorController_(
          Config::MotorPin,
          Config::DarkThreshold,
          Config::LightThreshold,
          Config::ConfirmTimeMs
      )
{
}

void Application::setup()
{
    logger_.begin();
    ldr_.init();
    motorController_.init();

    logger_.log("Ready. Motor: OFF");
}

void Application::loop()
{
    const uint32_t now = millis();

    if (ldr_.update(now))
    {
        motorController_.update(ldr_.filtered(), now);
    }

    if (now - lastPrintAt_ >= Config::PrintIntervalMs)
    {
        lastPrintAt_ = now;

        logger_.logf(
            "ADC raw: %u | avg: %u | Motor: %s | Pending: %s\n",
            static_cast<unsigned>(ldr_.raw()),
            static_cast<unsigned>(ldr_.filtered()),
            motorController_.state() == MotorState::On ? "ON" : "OFF",
            motorController_.isPending() ? "YES" : "NO"
        );
    }
}
