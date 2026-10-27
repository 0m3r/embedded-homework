#include <Arduino.h>

#include "lesson5_homework.h"

namespace lesson5 {
namespace homework {
namespace {
    constexpr uint8_t BUTTON_PIN = 21;
    constexpr uint16_t REPORT_INTERVAL_MS = 10;

    volatile uint32_t buttonCounter = 0;
    uint32_t lastCount = 0;

    void IRAM_ATTR buttonIsr() {
        buttonCounter++;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonIsr, FALLING);

    Serial.println("Contact bounce measurement ready");
    Serial.println("Button: GPIO21, interrupt: FALLING");
}

void loop() {
    const uint32_t currentCount = buttonCounter;

    if (currentCount != lastCount) {
        Serial.printf(
            "Falling edges: %lu (+%lu)\n",
            static_cast<unsigned long>(currentCount),
            static_cast<unsigned long>(currentCount - lastCount)
        );
        lastCount = currentCount;
    }

    delay(REPORT_INTERVAL_MS);
}
}
}
