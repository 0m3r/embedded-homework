#include "Application.hpp"

#include <Arduino.h>

void Application::setup()
{
    Serial.begin(Config::SerialBaud);
    button_.begin();
    led_.begin();

    if (!blinkTimer_.init(Config::BlinkIntervalMs)) {
        Serial.println("Hardware timer initialization failed");
    }
}

void Application::loop()
{
    if (button_.takePress()) {
        const auto now = millis();
        if (now - lastPressMs_ >= Config::DebounceMs) {
            lastPressMs_ = now;
            blinking_ = !blinking_;

            if (blinking_) {
                blinkTimer_.start();
                Serial.println("LED blinking started");
            } else {
                blinkTimer_.stop();
                led_.off();
                Serial.println("LED blinking stopped");
            }
        }
    }

    const auto periods = blinkTimer_.getPeriods();
    if (blinking_ && (periods % 2U) != 0U) {
        led_.toggle();
    }
}
