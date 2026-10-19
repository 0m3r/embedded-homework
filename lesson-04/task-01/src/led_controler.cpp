#include "led_controler.h"

namespace led_controler {
namespace {
constexpr unsigned long REVERSE_FLASH_MS = 150;
constexpr unsigned long REVERSE_OFF_MS = 300;

void flashReverseLeds(uint8_t pin, unsigned long flashDuration,
                      unsigned long offDuration) {
    pinMode(pin, OUTPUT);
    flashLeds(pin, flashDuration, flashDuration);

    // Reverse wiring uses HIGH for red, LOW for blue, and INPUT for off.
    pinMode(pin, INPUT);
    delay(offDuration);
}

void reverseLedTask(void *) {
    for (;;) {
        flashReverseLeds(REVERSE_PIN, REVERSE_FLASH_MS, REVERSE_OFF_MS);
    }
}
}

void initialize() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    pinMode(REVERSE_PIN, INPUT);
}

void flashLeds(std::initializer_list<uint8_t> pins, unsigned long duration,
               unsigned long gap) {
    for (uint8_t pin : pins) {
        digitalWrite(pin, HIGH);
    }

    delay(duration);

    for (uint8_t pin : pins) {
        digitalWrite(pin, LOW);
    }

    if (gap > 0) {
        delay(gap);
    }
}

void flashLeds(uint8_t pin, unsigned long duration, unsigned long gap) {
    flashLeds({pin}, duration, gap);
}

void startReverseTask() {
    xTaskCreate(reverseLedTask, "reverse-led", 2048, nullptr, 1, nullptr);
}
}
