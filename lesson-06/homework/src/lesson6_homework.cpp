#include <Arduino.h>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "lesson6_homework.h"

namespace lesson6 {
namespace homework {
namespace {
    constexpr uint8_t LDR_PIN = 2;
    constexpr uint16_t ADC_MAX = 4095;
    constexpr float REFERENCE_VOLTAGE_MV = 3100.0f;
    constexpr uint32_t MEASUREMENT_PERIOD_MS = 100;
    constexpr uint8_t ROWS_PER_HEADER = 25;

    uint32_t lastMeasurement = 0;
    uint8_t rowCount = 0;

    void printHeader() {
        std::cout << " RAW | CALC, mV | ADC, mV | ERROR, %" << std::endl;
        std::cout << "-----+----------+---------+---------" << std::endl;
    }
}

void setup() {
    pinMode(LDR_PIN, INPUT);
    Serial.begin(115200);
}

void loop() {
    const uint32_t now = millis();

    if (now - lastMeasurement < MEASUREMENT_PERIOD_MS) {
        return;
    }

    lastMeasurement = now;
    const uint16_t rawValue = analogRead(LDR_PIN);
    const float calculatedMillivolts =
        rawValue * REFERENCE_VOLTAGE_MV / ADC_MAX;
    const uint32_t measuredMillivolts = analogReadMilliVolts(LDR_PIN);

    if (rowCount == 0) {
        printHeader();
    }

    std::cout
        << std::setw(4) << rawValue
        << " | " << std::setw(8) << std::fixed << std::setprecision(1)
        << calculatedMillivolts
        << " | " << std::setw(7) << measuredMillivolts
        << " | ";

    if (measuredMillivolts == 0) {
        std::cout << std::setw(7) << "N/A";
    } else {
        const float errorPercent =
            std::fabs(calculatedMillivolts - measuredMillivolts)
            / measuredMillivolts
            * 100.0f;
        std::cout << std::setw(7) << std::setprecision(2) << errorPercent;
    }

    std::cout << std::endl;
    rowCount = (rowCount + 1) % ROWS_PER_HEADER;
}
}
}
