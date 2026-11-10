#pragma once

#include <stdint.h>

#include "Clock.h"
#include "Timer.h"

class HardwareSerial;

class UartExperiment
{
public:
    constexpr UartExperiment(
        const Clock& clock,
        HardwareSerial& uart,
        uint8_t txPin,
        uint32_t baudRate,
        uint32_t intervalMs
    )
        : uart_(uart),
          txPin_(txPin),
          baudRate_(baudRate),
          timer_(clock, intervalMs)
    {
    }

    void init();
    void update();
    uint8_t currentPattern() const;

private:
    HardwareSerial& uart_;
    const uint8_t txPin_;
    const uint32_t baudRate_;
    Timer timer_;
    uint8_t index_ = 0;
    uint8_t current_ = 0;
};
