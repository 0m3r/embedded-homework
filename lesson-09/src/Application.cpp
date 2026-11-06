#include "Application.h"

#include <Arduino.h>

#include "Config.h"
#include "LedMode.h"

Application::Application()
    : led_(Config::LedPin),
      button_(Config::ButtonPin, Config::DebounceMs),
      ledController_(led_, Config::BlinkIntervalMs),
      loopProfiler_(Config::LoopReportIterations)
{
}

void Application::setup()
{
    Serial.begin(Config::SerialBaud);

    led_.init();
    button_.init();
    ledController_.init(millis());

    Serial.println("Ready. Mode: BLINK");
}

void Application::loop()
{
    loopProfiler_.beginIteration(micros());

    const uint32_t now = millis();

    button_.update(now);

    if (button_.wasPressed())
    {
        ledController_.selectNextMode(now);

        Serial.printf(
            "Mode: %s\n",
            modeName(ledController_.mode())
        );
    }

    ledController_.update(now);

    if (loopProfiler_.endIteration(micros()))
    {
        Serial.printf(
            "Average loop time: %lu us (%lu iterations)\n",
            static_cast<unsigned long>(loopProfiler_.averageTimeUs()),
            static_cast<unsigned long>(loopProfiler_.iterationCount())
        );
    }
}
