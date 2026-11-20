#include "Esp32/Application.hpp"

#include <Arduino.h>

void Application::setup()
{
    Serial.begin(Config::SerialBaud);
    enableLoopWDT();

    led_.init();
    if (!button_.init()) {
        fail("Button initialization failed");
    }

    Serial.println("Debounce ready: GPIO21 -> GPIO15");
}

void Application::loop()
{
    feedLoopWDT();

    if (button_.takePress()) {
        led_.toggle();
        Serial.println(led_.isOn() ? "LED ON" : "LED OFF");
    }
}

void Application::fail(const char* message)
{
    Serial.println(message);
    while (true) {
    }
}
