#pragma once

#include <stdint.h>

class HardwareSerial;

class UartExperiment
{
public:
    constexpr UartExperiment(
        HardwareSerial& serial,
        uint8_t txPin,
        uint32_t baudRate,
        uint32_t intervalMs
    )
        : serial_(serial),
          txPin_(txPin),
          baudRate_(baudRate),
          intervalMs_(intervalMs)
    {
    }

    void init();
    void update(uint32_t nowMs);
    uint8_t currentPattern() const;

private:
    HardwareSerial& serial_;
    const uint8_t txPin_;
    const uint32_t baudRate_;
    const uint32_t intervalMs_;
    uint32_t lastSentAtMs_ = 0;
    uint8_t patternIndex_ = 0;
    uint8_t currentPattern_ = 0;
};
