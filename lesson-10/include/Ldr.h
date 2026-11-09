#pragma once

#include <stdint.h>

#include "Config.h"

class Ldr
{
public:
    constexpr Ldr(uint8_t pin, uint32_t sampleIntervalMs)
        : pin_(pin),
          sampleIntervalMs_(sampleIntervalMs)
    {
    }

    void init();
    bool update(uint32_t now);

    uint16_t raw() const;
    uint16_t filtered() const;

private:
    uint16_t filter(uint16_t value);

    const uint8_t pin_;
    const uint32_t sampleIntervalMs_;

    uint16_t readings_[Config::FilterSize] = {};
    uint32_t sum_ = 0;
    uint32_t lastSampleAt_ = 0;
    uint16_t raw_ = 0;
    uint16_t filtered_ = 0;
    uint8_t index_ = 0;
    uint8_t count_ = 0;
};
