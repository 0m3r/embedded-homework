#include <Arduino.h>
#include <initializer_list>

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

void flashLeds(std::initializer_list<uint8_t> pins,
               unsigned long duration = POLICE_FLASH_MS,
               unsigned long gap = POLICE_GAP_MS) {
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

void flashReverseLeds(uint8_t pin, unsigned long flashDuration,
                      unsigned long offDuration) {
    pinMode(pin, OUTPUT);
    flashLeds({pin}, flashDuration, flashDuration);

    // Reverse wiring uses HIGH for red, LOW for blue, and INPUT for off.
    pinMode(pin, INPUT);
    delay(offDuration);

    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
}

void sendMorseLetter(uint8_t firstPin, uint8_t secondPin,
                     const char *pattern) {
    for (size_t i = 0; pattern[i] != '\0'; ++i) {
        const unsigned long duration =
            pattern[i] == '.' ? MORSE_UNIT_MS : MORSE_UNIT_MS * 3;

        flashLeds({firstPin, secondPin}, duration, 0);

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
            flashLeds({RED_LED_PIN});
            flashLeds({BLUE_LED_PIN});
        }
    }

    if (ACTIVE_MODES & MODE_2) {
        for (uint8_t i = 0; i < 3; ++i) {
            flashLeds({RED_LED_PIN});
        }

        delay(POLICE_GROUP_GAP_MS);

        for (uint8_t i = 0; i < 3; ++i) {
            flashLeds({BLUE_LED_PIN});
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
