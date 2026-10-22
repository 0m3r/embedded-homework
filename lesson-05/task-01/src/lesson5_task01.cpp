#include <Arduino.h>

#include "lesson5_task01.h"

namespace lesson5 {
namespace task01 {
namespace {
    constexpr uint8_t LED_RED_OUTPUT = 15;
    constexpr uint8_t LED_GREEN_OUTPUT = 16;
    constexpr uint8_t EXT_BUTTON_IN = 21;
    constexpr uint8_t BOOT_BUTTON_IN = 0;

    constexpr uint16_t DELAY_FAST_MS = 100;
    constexpr uint16_t DELAY_SLOW_MS = 500;
    constexpr uint16_t DELAY_EXTRA_MS = 1000;
    constexpr uint16_t DEBOUNCE_DELAY_MS = 150;
    constexpr uint16_t SMART_DELAY_STEP_MS = 10;

    enum class Mode : uint8_t {
        Fast,
        Slow,
        Extra,
    };

    enum class ButtonState : uint8_t {
        None,
        External,
        Boot,
        Both,
    };

    Mode currentMode = Mode::Slow;
    ButtonState lastRawButtonState = ButtonState::None;
    ButtonState stableButtonState = ButtonState::None;
    uint32_t buttonStateChangedAt = 0;

    ButtonState readButtonState() {
        const bool isExternalPressed = digitalRead(EXT_BUTTON_IN) == LOW;
        const bool isBootPressed = digitalRead(BOOT_BUTTON_IN) == LOW;

        if (isExternalPressed && isBootPressed) {
            return ButtonState::Both;
        }
        if (isExternalPressed) {
            return ButtonState::External;
        }
        if (isBootPressed) {
            return ButtonState::Boot;
        }
        return ButtonState::None;
    }

    void setMode(Mode mode) {
        if (currentMode == mode) {
            return;
        }

        currentMode = mode;

        switch (currentMode) {
            case Mode::Fast:
                Serial.println("Fast mode, delay = 100");
                break;
            case Mode::Slow:
                Serial.println("Slow mode, delay = 500");
                break;
            case Mode::Extra:
                Serial.println("Extra mode, delay = 1000");
                break;
        }
    }

    void checkButtons() {
        const ButtonState rawButtonState = readButtonState();
        const uint32_t now = millis();

        if (rawButtonState != lastRawButtonState) {
            lastRawButtonState = rawButtonState;
            buttonStateChangedAt = now;
        }

        if (
            rawButtonState == stableButtonState
            || now - buttonStateChangedAt < DEBOUNCE_DELAY_MS
        ) {
            return;
        }

        stableButtonState = rawButtonState;
        const bool isExternalPressed =
            stableButtonState == ButtonState::External
            || stableButtonState == ButtonState::Both;
        const bool isBootPressed =
            stableButtonState == ButtonState::Boot
            || stableButtonState == ButtonState::Both;

        Serial.printf(
            "GPIO%d: %s | GPIO%d: %s\n",
            static_cast<int>(EXT_BUTTON_IN),
            isExternalPressed ? "PRESSED" : "RELEASED",
            static_cast<int>(BOOT_BUTTON_IN),
            isBootPressed ? "PRESSED" : "RELEASED"
        );

        switch (stableButtonState) {
            case ButtonState::External:
                setMode(Mode::Fast);
                break;
            case ButtonState::Boot:
                setMode(Mode::Slow);
                break;
            case ButtonState::Both:
                setMode(Mode::Extra);
                break;
            case ButtonState::None:
                break;
        }
    }

    bool smartDelay(uint32_t duration) {
        const Mode modeAtStart = currentMode;

        for (uint32_t elapsed = 0; elapsed < duration; elapsed += SMART_DELAY_STEP_MS) {
            checkButtons();
            if (currentMode != modeAtStart) {
                return false;
            }
            delay(SMART_DELAY_STEP_MS);
        }

        return true;
    }

    uint32_t getCurrentDelay() {
        switch (currentMode) {
            case Mode::Fast:
                return DELAY_FAST_MS;
            case Mode::Slow:
                return DELAY_SLOW_MS;
            case Mode::Extra:
                return DELAY_EXTRA_MS;
        }

        return DELAY_SLOW_MS;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_RED_OUTPUT, OUTPUT);
    pinMode(LED_GREEN_OUTPUT, OUTPUT);
    digitalWrite(LED_RED_OUTPUT, LOW);
    digitalWrite(LED_GREEN_OUTPUT, LOW);

    pinMode(EXT_BUTTON_IN, INPUT_PULLUP);
    pinMode(BOOT_BUTTON_IN, INPUT_PULLUP);
}

void loop() {
    const uint32_t currentDelay = getCurrentDelay();

    digitalWrite(LED_RED_OUTPUT, HIGH);
    digitalWrite(LED_GREEN_OUTPUT, LOW);

    if (!smartDelay(currentDelay)) {
        return;
    }

    digitalWrite(LED_RED_OUTPUT, LOW);
    digitalWrite(LED_GREEN_OUTPUT, HIGH);

    smartDelay(currentDelay);
}
}
}
