#pragma once

#include <cstdint>

class Logger {
public:
    void begin(std::uint32_t baud) const;
    void started(std::uint8_t pin) const;
    void press(const char* method, std::uint32_t count) const;
};
