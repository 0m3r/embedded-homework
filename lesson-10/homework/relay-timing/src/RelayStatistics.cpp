#include "RelayStatistics.h"

void RelayStatistics::reset()
{
    cycles_ = 0;
    onTotalUs_ = 0;
    offTotalUs_ = 0;
}

void RelayStatistics::addOn(uint32_t durationUs)
{
    onTotalUs_ += durationUs;
}

void RelayStatistics::addOff(uint32_t durationUs)
{
    offTotalUs_ += durationUs;
    ++cycles_;
}

uint8_t RelayStatistics::completedCycles() const
{
    return cycles_;
}

uint32_t RelayStatistics::averageOnUs() const
{
    return cycles_ == 0
        ? 0
        : static_cast<uint32_t>(onTotalUs_ / cycles_);
}

uint32_t RelayStatistics::averageOffUs() const
{
    return cycles_ == 0
        ? 0
        : static_cast<uint32_t>(offTotalUs_ / cycles_);
}
