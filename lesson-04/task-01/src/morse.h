#pragma once

#include <Arduino.h>
#include <initializer_list>

namespace morse {
    void send(const char *message, std::initializer_list<uint8_t> pins);
}
