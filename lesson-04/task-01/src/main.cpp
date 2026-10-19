#include <Arduino.h>

namespace {
constexpr uint8_t RED_LED_PIN = 15;
constexpr uint8_t BLUE_LED_PIN = 40;

constexpr unsigned long POLICE_FLASH_MS = 120;
constexpr unsigned long POLICE_GAP_MS = 80;
constexpr unsigned long MORSE_UNIT_MS = 180;

void flashLed(uint8_t pin, unsigned long duration, unsigned long gap) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(gap);
}

void flashBoth(unsigned long duration) {
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, HIGH);
  delay(duration);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void sendMorseLetter(const char *pattern) {
  for (size_t i = 0; pattern[i] != '\0'; ++i) {
    const unsigned long duration =
        pattern[i] == '.' ? MORSE_UNIT_MS : MORSE_UNIT_MS * 3;
    flashBoth(duration);

    if (pattern[i + 1] != '\0') {
      delay(MORSE_UNIT_MS);
    }
  }
}

void sendSos() {
  sendMorseLetter("...");
  delay(MORSE_UNIT_MS * 3);
  sendMorseLetter("---");
  delay(MORSE_UNIT_MS * 3);
  sendMorseLetter("...");
  delay(MORSE_UNIT_MS * 7);
}
}

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void loop() {
  for (uint8_t i = 0; i < 4; ++i) {
    flashLed(RED_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
    flashLed(BLUE_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
  }

  delay(300);
  sendSos();
}
