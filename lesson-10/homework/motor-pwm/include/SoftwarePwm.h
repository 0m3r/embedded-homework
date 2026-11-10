#pragma once

#include <stdint.h>

#include "Clock.h"

class SoftwarePwm
{
public:
    constexpr SoftwarePwm(
        const Clock& clock,
        uint8_t pin,
        uint32_t periodUs
    )
        : clock_(clock),
          pin_(pin),
          periodUs_(periodUs)
    {
    }

    void init();
    void setDuty(uint8_t duty);
    void update();
    uint8_t duty() const;

private:
    void setState(bool state);

    const Clock& clock_;
    const uint8_t pin_;
    const uint32_t periodUs_;
    uint32_t cycleStartedAtUs_ = 0;
    uint8_t duty_ = 0;
    bool state_ = false;
};
