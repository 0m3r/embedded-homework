#include "morse.h"

#include "led_controler.h"

namespace morse {
namespace {
constexpr unsigned long UNIT_MS = 180;

struct Symbol {
    char value;
    const char *pattern;
};

constexpr Symbol ALPHABET[] = {
    {'S', "..."},
    {'O', "---"},
};

char normalize(char value) {
    if (value >= 'a' && value <= 'z') {
        return value - ('a' - 'A');
    }

    return value;
}

const char *findPattern(char value) {
    const char normalized = normalize(value);

    for (const Symbol &symbol : ALPHABET) {
        if (symbol.value == normalized) {
            return symbol.pattern;
        }
    }

    return nullptr;
}

void sendLetter(const char *pattern, std::initializer_list<uint8_t> pins) {
    for (size_t i = 0; pattern[i] != '\0'; ++i) {
        const unsigned long duration =
            pattern[i] == '.' ? UNIT_MS : UNIT_MS * 3;

        led_controler::flashLeds(pins, duration, 0);

        if (pattern[i + 1] != '\0') {
            delay(UNIT_MS);
        }
    }
}
}

void send(const char *message, std::initializer_list<uint8_t> pins) {
    bool hasPreviousLetter = false;

    for (size_t i = 0; message[i] != '\0'; ++i) {
        if (message[i] == ' ') {
            if (hasPreviousLetter) {
                delay(UNIT_MS * 7);
                hasPreviousLetter = false;
            }
            continue;
        }

        const char *pattern = findPattern(message[i]);
        if (pattern == nullptr) {
            continue;
        }

        if (hasPreviousLetter) {
            delay(UNIT_MS * 3);
        }

        sendLetter(pattern, pins);
        hasPreviousLetter = true;
    }

    if (hasPreviousLetter) {
        delay(UNIT_MS * 7);
    }
}
}
