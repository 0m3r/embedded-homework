#include <Arduino.h>

namespace {
constexpr uint8_t RED_LED_PIN = 15;
constexpr uint8_t BLUE_LED_PIN = 40;
constexpr uint8_t REVERSE_LED_PIN = 4;

constexpr unsigned long POLICE_FLASH_MS = 120;
constexpr unsigned long POLICE_GAP_MS = 80;
constexpr unsigned long MORSE_UNIT_MS = 180;

enum Mode : uint8_t {
    MODE_1 = 1 << 0,
    MODE_2 = 1 << 1,
    MODE_3 = 1 << 2,
    MODE_SOS = 1 << 3,
};

constexpr uint8_t ACTIVE_MODES = MODE_1 | MODE_3 | MODE_SOS;

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

void flashReverseLeds() {
    // Червоний через GPIO4
    digitalWrite(REVERSE_LED_PIN, HIGH);
    delay(150);

    // Синій через GPIO4
    digitalWrite(REVERSE_LED_PIN, LOW);
    delay(150);

    // Обидва вимкнені
    pinMode(REVERSE_LED_PIN, INPUT);
    delay(300);

    // Повернення до OUTPUT без зайвого спалаху
    digitalWrite(REVERSE_LED_PIN, HIGH);
    pinMode(REVERSE_LED_PIN, OUTPUT);
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

    // Починаємо з вимкнених світлодіодів GPIO4
    pinMode(REVERSE_LED_PIN, INPUT);
}

void loop() {
    if (ACTIVE_MODES & MODE_1) {
        for (uint8_t i = 0; i < 4; ++i) {
            flashLed(RED_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
            flashLed(BLUE_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
        }
    }

    if (ACTIVE_MODES & MODE_2) {
        for (uint8_t i = 0; i < 3; ++i) {
            flashLed(RED_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
        }

        delay(150);

        for (uint8_t i = 0; i < 3; ++i) {
            flashLed(BLUE_LED_PIN, POLICE_FLASH_MS, POLICE_GAP_MS);
        }

        delay(300);
    }

    if (ACTIVE_MODES & MODE_3) {
        flashReverseLeds();
    }

    if (ACTIVE_MODES & MODE_SOS) {
        delay(300);
        sendSos();
    }
}
