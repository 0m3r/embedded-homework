#include "UartExperiment.h"

#include <Arduino.h>

namespace
{
constexpr uint8_t Patterns[] = {0x00, 0x0F, 0x55, 0xFF};
constexpr uint8_t PatternCount = sizeof(Patterns) / sizeof(Patterns[0]);
}

void UartExperiment::init()
{
    uart_.begin(baudRate_, SERIAL_8N1, -1, txPin_);
}

void UartExperiment::update()
{
    if (!timer_.ready())
    {
        return;
    }

    current_ = Patterns[index_];
    uart_.write(current_);
    index_ = (index_ + 1) % PatternCount;
}

uint8_t UartExperiment::currentPattern() const
{
    return current_;
}
