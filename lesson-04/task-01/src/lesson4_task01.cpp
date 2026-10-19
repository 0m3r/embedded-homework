#include <Arduino.h>
#include <initializer_list>

#include "led_controler.h"
#include "lesson4_task01.h"
#include "morse.h"

namespace lesson4 {
namespace task01 {
namespace {
    enum Mode : uint8_t {
        MODE_ALTERNATING = 1 << 0,
        MODE_GROUPED = 1 << 1,
        MODE_REVERSE = 1 << 2,
        MODE_SOS = 1 << 3,
    };

    constexpr uint8_t ACTIVE_MODES = MODE_GROUPED | MODE_REVERSE | MODE_SOS;

    const std::initializer_list<uint8_t> MORSE_PINS = {
        led_controler::RED_PIN,
        led_controler::BLUE_PIN,
    };
}

void setup() {
    led_controler::initialize();

    if (ACTIVE_MODES & MODE_REVERSE) {
        led_controler::startReverseTask();
    }
}

void loop() {
    if (ACTIVE_MODES & MODE_ALTERNATING) {
        for (uint8_t i = 0; i < 4; ++i) {
            led_controler::flashLeds(led_controler::RED_PIN);
            led_controler::flashLeds(led_controler::BLUE_PIN);
        }
    }

    if (ACTIVE_MODES & MODE_GROUPED) {
        for (uint8_t i = 0; i < 3; ++i) {
            led_controler::flashLeds(led_controler::RED_PIN);
        }

        delay(led_controler::POLICE_GROUP_GAP_MS);

        for (uint8_t i = 0; i < 3; ++i) {
            led_controler::flashLeds(led_controler::BLUE_PIN);
        }

        delay(led_controler::MODE_GAP_MS);
    }

    if (ACTIVE_MODES & MODE_SOS) {
        delay(led_controler::MODE_GAP_MS);
        morse::send("SOS", MORSE_PINS);
    }
}
}
}
