#include <Arduino.h>

#include "lesson4_homework.h"

namespace lesson4 {
namespace homework {
namespace {
    constexpr uint8_t FIRST_LED_PIN = 15;
    constexpr uint8_t SECOND_LED_PIN = 16;
    constexpr uint8_t EXTERNAL_BUTTON_PIN = 21;
    constexpr uint8_t BOOT_BUTTON_PIN = 0;
    constexpr uint16_t FAST_INTERVAL_MS = 200;
    constexpr uint16_t SLOW_INTERVAL_MS = 1000;

    enum class Mode : uint8_t {
        Synchronous,
        Alternating,
    };

    Mode activeMode = Mode::Synchronous;
    bool wasExternalButtonPressed = false;
    bool wasBootButtonPressed = false;
    bool ledPhase = true;
    uint32_t lastLedUpdateMs = 0;

    void logButtonState(uint8_t pin, bool isPressed, bool& wasPressed) {
        if (isPressed == wasPressed) {
            return;
        }

        Serial.printf(
            "Button GPIO%d: %s\n",
            static_cast<int>(pin),
            isPressed ? "PRESSED" : "RELEASED"
        );
        wasPressed = isPressed;
    }

    void setLeds(uint8_t firstState, uint8_t secondState) {
        digitalWrite(FIRST_LED_PIN, firstState);
        digitalWrite(SECOND_LED_PIN, secondState);
    }

    void resetLedPattern() {
        ledPhase = true;
        lastLedUpdateMs = millis();

        if (activeMode == Mode::Synchronous) {
            setLeds(HIGH, HIGH);
        } else {
            setLeds(HIGH, LOW);
        }
    }

    void setMode(Mode mode) {
        if (activeMode == mode) {
            return;
        }

        activeMode = mode;
        Serial.println(
            activeMode == Mode::Synchronous
                ? "Mode: Synchronous"
                : "Mode: Alternating"
        );
        resetLedPattern();
    }

    void updateMode() {
        const bool isExternalButtonPressed = digitalRead(EXTERNAL_BUTTON_PIN) == LOW;
        const bool isBootButtonPressed = digitalRead(BOOT_BUTTON_PIN) == LOW;

        logButtonState(
            EXTERNAL_BUTTON_PIN,
            isExternalButtonPressed,
            wasExternalButtonPressed
        );
        logButtonState(
            BOOT_BUTTON_PIN,
            isBootButtonPressed,
            wasBootButtonPressed
        );

        if (isExternalButtonPressed) {
            setMode(Mode::Synchronous);
        } else if (isBootButtonPressed) {
            setMode(Mode::Alternating);
        }
    }

    void updateLeds() {
        const uint32_t now = millis();
        const uint16_t interval = activeMode == Mode::Synchronous
            ? FAST_INTERVAL_MS
            : SLOW_INTERVAL_MS;

        if (now - lastLedUpdateMs < interval) {
            return;
        }

        lastLedUpdateMs = now;
        ledPhase = !ledPhase;

        if (activeMode == Mode::Synchronous) {
            setLeds(ledPhase ? HIGH : LOW, ledPhase ? HIGH : LOW);
        } else {
            setLeds(ledPhase ? HIGH : LOW, ledPhase ? LOW : HIGH);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(FIRST_LED_PIN, OUTPUT);
    pinMode(SECOND_LED_PIN, OUTPUT);
    pinMode(EXTERNAL_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
    resetLedPattern();
}

void loop() {
    updateMode();
    updateLeds();
}
}
}
