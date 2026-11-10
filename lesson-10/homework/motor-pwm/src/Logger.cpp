#include "Logger.h"

#include <Arduino.h>
#include <stdarg.h>

Logger::Logger(HardwareSerial& serial, uint32_t baudRate)
    : serial_(serial),
      baudRate_(baudRate)
{
}

void Logger::begin()
{
    serial_.begin(baudRate_);
}

void Logger::end()
{
    serial_.end();
}

void Logger::log(const char* message)
{
    serial_.println(message);
}

void Logger::logf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    serial_.vprintf(format, args);
    va_end(args);
}
