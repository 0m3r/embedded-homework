#include "Logger.h"

#include <Arduino.h>

Logger::Logger(HardwareSerial& serial, uint32_t baudRate)
    : serial_(serial),
      baudRate_(baudRate)
{
}

void Logger::begin()
{
    serial_.begin(baudRate_);
}

void Logger::log(const char* message)
{
    serial_.println(message);
}
