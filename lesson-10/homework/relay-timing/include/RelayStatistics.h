#pragma once

#include <stdint.h>

class RelayStatistics
{
public:
    void reset();
    void addOn(uint32_t durationUs);
    void addOff(uint32_t durationUs);

    uint8_t completedCycles() const;
    uint32_t averageOnUs() const;
    uint32_t averageOffUs() const;

private:
    uint8_t cycles_ = 0;
    uint64_t onTotalUs_ = 0;
    uint64_t offTotalUs_ = 0;
};
