#pragma once

#include <stdint.h>

class HardwareSerial;

class Logger
{
public:
    Logger(HardwareSerial& serial, uint32_t baudRate);

    void begin();
    void end();
    void log(const char* message);
    void logf(const char* format, ...);

private:
    HardwareSerial& serial_;
    const uint32_t baudRate_;
};
