#include <Arduino.h>

#include "Button.h"
#include "Config.h"
#include "Led.h"
#include "LedController.h"
#include "LedMode.h"
#include "LoopProfiler.h"

Led& getLed()
{
    static Led led(Config::LedPin);
    return led;
}

Button& getButton()
{
    static Button button(Config::ButtonPin, Config::DebounceMs);
    return button;
}

LedController& getLedController()
{
    static LedController controller(getLed(), Config::BlinkIntervalMs);
    return controller;
}

LoopProfiler& getLoopProfiler()
{
    static LoopProfiler profiler(Config::LoopReportIterations);
    return profiler;
}

void setup()
{
    Serial.begin(Config::SerialBaud);

    getLed().init();
    getButton().init();
    getLedController().init(millis());

    Serial.println("Ready. Mode: BLINK");
}

void loop()
{
    LoopProfiler& profiler = getLoopProfiler();
    profiler.beginIteration(micros());

    const uint32_t now = millis();

    Button& button = getButton();
    LedController& ledController = getLedController();

    button.update(now);

    if (button.wasPressed())
    {
        ledController.selectNextMode(now);

        Serial.printf("Mode: %s\n", modeName(ledController.mode()));
    }

    ledController.update(now);

    if (profiler.endIteration(micros()))
    {
        Serial.printf(
            "Average loop time: %lu us (%lu iterations)\n",
            static_cast<unsigned long>(profiler.averageTimeUs()),
            static_cast<unsigned long>(profiler.iterationCount())
        );
    }
}
