#include <Arduino.h>

#include "lesson4_task02.h"

namespace lesson4 {
namespace task02 {
namespace {
    constexpr uint8_t LED_PIN = 15;
    constexpr uint8_t BUTTON_PIN = 0;
    constexpr uint16_t LOOP_DELAY_MS = 100;
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(115200);
    delay(1000);
}

void loop() {
    const bool isButtonPressed = digitalRead(BUTTON_PIN) == LOW;

    digitalWrite(LED_PIN, isButtonPressed ? HIGH : LOW);
    Serial.println(isButtonPressed ? "Button pressed" : "Button released");
    delay(LOOP_DELAY_MS);
}
}
}
