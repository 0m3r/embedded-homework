#include "Logger.hpp"

#include <Arduino.h>

void Logger::begin(std::uint32_t baud) const
{
    Serial.begin(baud);
}

void Logger::started(std::uint8_t pin) const
{
    Serial.printf("All debounce methods active on GPIO%u\n", pin);
}

void Logger::press(const char* method, std::uint32_t count) const
{
    Serial.printf("%s: %lu\n", method, static_cast<unsigned long>(count));
}
