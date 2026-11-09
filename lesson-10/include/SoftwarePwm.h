#pragma once

#include <stdint.h>

class SoftwarePwm
{
public:
    constexpr SoftwarePwm(uint8_t pin, uint32_t periodUs)
        : pin_(pin),
          periodUs_(periodUs)
    {
    }

    void init(uint32_t nowUs);
    void setDuty(uint8_t duty);
    void update(uint32_t nowUs);
    uint8_t duty() const;

private:
    void write(bool high);

    const uint8_t pin_;
    const uint32_t periodUs_;
    uint32_t cycleStartedAtUs_ = 0;
    uint8_t duty_ = 0;
    bool outputHigh_ = false;
};
