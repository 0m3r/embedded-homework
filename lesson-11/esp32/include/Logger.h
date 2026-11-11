#pragma once

#include <stdint.h>

class HardwareSerial;

class Logger
{
public:
    Logger(HardwareSerial& serial, uint32_t baudRate);
    void begin();
    void log(const char* message);

private:
    HardwareSerial& serial_;
    const uint32_t baudRate_;
};
