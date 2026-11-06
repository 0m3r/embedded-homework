#include "Application.h"

#include <Arduino.h>

#include "Config.h"
#include "LedMode.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      button_(Config::ButtonPin, Config::DebounceMs),
      ledController_(Config::LedPin, Config::BlinkIntervalMs),
      loopProfiler_(Config::LoopReportIterations)
{
}

void Application::setup()
{
    logger_.begin();

    button_.init();
    ledController_.init(millis());

    logger_.logf(
        "Ready. Mode: %s\n",
        modeName(ledController_.mode())
    );
}

void Application::loop()
{
    loopProfiler_.beginIteration(micros());

    const uint32_t now = millis();

    button_.update(now);

    if (button_.wasPressed())
    {
        ledController_.next(now);

        logger_.logf(
            "Mode: %s\n",
            modeName(ledController_.mode())
        );
    }

    ledController_.update(now);

    if (loopProfiler_.endIteration(micros()))
    {
        logger_.logf(
            "Average loop time: %lu us (%lu iterations)\n",
            static_cast<unsigned long>(loopProfiler_.averageTimeUs()),
            static_cast<unsigned long>(loopProfiler_.iterationCount())
        );
    }
}
