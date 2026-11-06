#pragma once

#include <stdint.h>

class Watchdog
{
public:
    void init(uint32_t timeoutMs);
    void feed();
};
