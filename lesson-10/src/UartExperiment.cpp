#include "UartExperiment.h"

#include <Arduino.h>

namespace
{
constexpr uint8_t Patterns[] = {0x00, 0x0F, 0x55, 0xFF};
constexpr uint8_t PatternCount = sizeof(Patterns) / sizeof(Patterns[0]);
}

void UartExperiment::init()
{
    serial_.begin(baudRate_, SERIAL_8N1, -1, txPin_);
}

void UartExperiment::update(uint32_t nowMs)
{
    if (nowMs - lastSentAtMs_ < intervalMs_)
    {
        return;
    }

    lastSentAtMs_ = nowMs;
    currentPattern_ = Patterns[patternIndex_];
    serial_.write(currentPattern_);
    patternIndex_ = (patternIndex_ + 1) % PatternCount;
}

uint8_t UartExperiment::currentPattern() const
{
    return currentPattern_;
}
