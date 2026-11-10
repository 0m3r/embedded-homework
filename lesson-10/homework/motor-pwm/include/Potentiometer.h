#pragma once

#include <stdint.h>

class Potentiometer
{
public:
    constexpr Potentiometer(uint8_t pin, uint32_t sampleIntervalMs)
        : pin_(pin),
          sampleIntervalMs_(sampleIntervalMs)
    {
    }

    void init();
    bool update(uint32_t nowMs);
    uint16_t value() const;
    uint8_t duty() const;

private:
    const uint8_t pin_;
    const uint32_t sampleIntervalMs_;
    uint32_t lastSampleAtMs_ = 0;
    uint16_t value_ = 0;
};
