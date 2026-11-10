#pragma once

#include <stdint.h>

class Clock
{
public:
    void update();

    uint32_t nowMs() const;
    uint32_t nowUs() const;

private:
    uint32_t nowMs_ = 0;
    uint32_t nowUs_ = 0;
};
