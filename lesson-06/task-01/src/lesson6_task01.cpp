#include <Arduino.h>
#include <iomanip>
#include <iostream>

#include "hysteresis.h"
#include "lesson6_task01.h"
#include "median_filter.h"
#include "moving_average.h"

namespace lesson6 {
namespace task01 {
namespace {
    constexpr uint8_t LED_PIN = 17;
    constexpr uint8_t LDR_PIN = 2;
    constexpr uint16_t LED_ON_THRESHOLD = 2800;
    constexpr uint16_t LED_OFF_THRESHOLD = 3200;
    constexpr uint32_t ADC_PERIOD_MS = 10;
    constexpr uint32_t DEBUG_PERIOD_MS = 500;

    uint32_t lastTimeChange = 0;
    uint32_t lastDebugOutput = 0;
    uint8_t debugRowCount = 0;
    Hysteresis ledHysteresis(LED_ON_THRESHOLD, LED_OFF_THRESHOLD);
    MovingAverage movingAverage;
    MedianFilter medianFilter;

    void updateLed(uint16_t adcValue) {
        digitalWrite(LED_PIN, ledHysteresis.update(adcValue) ? HIGH : LOW);
    }
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(LDR_PIN, INPUT);
    Serial.begin(115200);
    Serial.println("Setup complete");

    const uint16_t initialValue = analogRead(LDR_PIN);
    movingAverage.reset(initialValue);
    medianFilter.reset(initialValue);
    updateLed(initialValue);
}

void loop() {
    const uint32_t now = millis();

    if (now - lastTimeChange < ADC_PERIOD_MS) {
        return;
    }

    lastTimeChange = now;
    const uint16_t adcValue = analogRead(LDR_PIN);
    const uint16_t averageValue = movingAverage.update(adcValue);
    const uint16_t medianValue = medianFilter.update(adcValue);

    updateLed(averageValue);

    if (now - lastDebugOutput >= DEBUG_PERIOD_MS) {
        lastDebugOutput = now;

        if (debugRowCount == 0) {
            std::cout << "-----+---------+--------+-----" << std::endl;
            std::cout << " RAW | AVERAGE | MEDIAN |  LED" << std::endl;
            std::cout << "-----+---------+--------+-----" << std::endl;
        }

        std::cout
            << std::setw(4) << adcValue
            << " | " << std::setw(7) << averageValue
            << " | " << std::setw(6) << medianValue
            << " | " << std::setw(4) << (ledHysteresis.isOn() ? "ON" : "OFF")
            << std::endl;

        debugRowCount = (debugRowCount + 1) % 20;
    }
}
}
}
