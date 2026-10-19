#include <Arduino.h>

namespace {
constexpr uint8_t RED_LED_PIN = 15;
constexpr uint8_t BLUE_LED_PIN = 40;
constexpr uint8_t REVERSE_LED_PIN = 4;

constexpr unsigned long POLICE_FLASH_MS = 120;
constexpr unsigned long POLICE_GAP_MS = 80;
constexpr unsigned long POLICE_GROUP_GAP_MS = 150;
constexpr unsigned long MODE_GAP_MS = 300;
constexpr unsigned long REVERSE_FLASH_MS = 150;
constexpr unsigned long REVERSE_OFF_MS = 300;
constexpr unsigned long MORSE_UNIT_MS = 180;

enum Mode : uint8_t {
    MODE_1 = 1 << 0,
    MODE_2 = 1 << 1,
    MODE_3 = 1 << 2,
    MODE_SOS = 1 << 3,
};

constexpr uint8_t ACTIVE_MODES = MODE_1 | MODE_3 | MODE_SOS;

void flashLed(uint8_t pin, unsigned long duration = POLICE_FLASH_MS,
              unsigned long gap = POLICE_GAP_MS) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(gap);
}

void flashBoth(uint8_t firstPin, uint8_t secondPin, unsigned long duration) {
    digitalWrite(firstPin, HIGH);
    digitalWrite(secondPin, HIGH);
    delay(duration);
    digitalWrite(firstPin, LOW);
    digitalWrite(secondPin, LOW);
}

void flashReverseLeds(uint8_t pin, unsigned long flashDuration,
                      unsigned long offDuration) {
    // Червоний через GPIO4
    digitalWrite(pin, HIGH);
    delay(flashDuration);

    // Синій через GPIO4
    digitalWrite(pin, LOW);
    delay(flashDuration);

    // Обидва вимкнені
    pinMode(pin, INPUT);
    delay(offDuration);

    // Повернення до OUTPUT без зайвого спалаху
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
}

void sendMorseLetter(uint8_t firstPin, uint8_t secondPin,
                     const char *pattern) {
    for (size_t i = 0; pattern[i] != '\0'; ++i) {
        const unsigned long duration =
            pattern[i] == '.' ? MORSE_UNIT_MS : MORSE_UNIT_MS * 3;

        flashBoth(firstPin, secondPin, duration);

        if (pattern[i + 1] != '\0') {
            delay(MORSE_UNIT_MS);
        }
    }
}

void sendSos(uint8_t firstPin, uint8_t secondPin) {
    sendMorseLetter(firstPin, secondPin, "...");
    delay(MORSE_UNIT_MS * 3);
    sendMorseLetter(firstPin, secondPin, "---");
    delay(MORSE_UNIT_MS * 3);
    sendMorseLetter(firstPin, secondPin, "...");
    delay(MORSE_UNIT_MS * 7);
}
}

void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);

    // Починаємо з вимкнених світлодіодів GPIO4
    pinMode(REVERSE_LED_PIN, INPUT);
}

void loop() {
    if (ACTIVE_MODES & MODE_1) {
        for (uint8_t i = 0; i < 4; ++i) {
            flashLed(RED_LED_PIN);
            flashLed(BLUE_LED_PIN);
        }
    }

    if (ACTIVE_MODES & MODE_2) {
        for (uint8_t i = 0; i < 3; ++i) {
            flashLed(RED_LED_PIN);
        }

        delay(POLICE_GROUP_GAP_MS);

        for (uint8_t i = 0; i < 3; ++i) {
            flashLed(BLUE_LED_PIN);
        }

        delay(MODE_GAP_MS);
    }

    if (ACTIVE_MODES & MODE_3) {
        flashReverseLeds(REVERSE_LED_PIN, REVERSE_FLASH_MS, REVERSE_OFF_MS);
    }

    if (ACTIVE_MODES & MODE_SOS) {
        delay(MODE_GAP_MS);
        sendSos(RED_LED_PIN, BLUE_LED_PIN);
    }
}
