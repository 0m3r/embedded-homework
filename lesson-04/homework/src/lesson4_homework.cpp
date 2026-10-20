#include <Arduino.h>

#include "lesson4_homework.h"

namespace lesson4 {
namespace homework {
namespace {
    constexpr uint8_t FIRST_LED_PIN = 15;
    constexpr uint8_t SECOND_LED_PIN = 16;
    constexpr uint8_t EXTERNAL_BUTTON_PIN = 21;
    constexpr uint8_t BOOT_BUTTON_PIN = 0;
    constexpr uint16_t FAST_DELAY_MS = 200;
    constexpr uint16_t SLOW_DELAY_MS = 1000;

    enum class Mode : uint8_t {
        Synchronous,
        Alternating,
    };

    Mode activeMode = Mode::Synchronous;

    void updateMode() {
        if (digitalRead(EXTERNAL_BUTTON_PIN) == LOW) {
            activeMode = Mode::Synchronous;
        } else if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
            activeMode = Mode::Alternating;
        }
    }

    void setLeds(uint8_t firstState, uint8_t secondState) {
        digitalWrite(FIRST_LED_PIN, firstState);
        digitalWrite(SECOND_LED_PIN, secondState);
    }

    void syncMode() {
        setLeds(HIGH, HIGH);
        delay(FAST_DELAY_MS);
        setLeds(LOW, LOW);
        delay(FAST_DELAY_MS);
    }

    void asyncMode() {
        setLeds(HIGH, LOW);
        delay(SLOW_DELAY_MS);
        setLeds(LOW, HIGH);
        delay(SLOW_DELAY_MS);
    }
}

void setup() {
    pinMode(FIRST_LED_PIN, OUTPUT);
    pinMode(SECOND_LED_PIN, OUTPUT);
    pinMode(EXTERNAL_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BOOT_BUTTON_PIN, INPUT);
    setLeds(LOW, LOW);
}

void loop() {
    updateMode();

    if (activeMode == Mode::Synchronous) {
        syncMode();
    } else {
        asyncMode();
    }
}
}
}
