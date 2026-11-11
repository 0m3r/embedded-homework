#pragma once

#include <stdint.h>

class Clock
{
public:
    void update();
    uint32_t nowMs() const;

private:
    uint32_t nowMs_ = 0;
};
