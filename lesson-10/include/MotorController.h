#pragma once

#include <stdint.h>

#include "Motor.h"

class MotorController
{
public:
    constexpr MotorController(
        uint8_t motorPin,
        uint16_t darkThreshold,
        uint16_t lightThreshold,
        uint32_t confirmTimeMs
    )
        : motor_(motorPin),
          darkThreshold_(darkThreshold),
          lightThreshold_(lightThreshold),
          confirmTimeMs_(confirmTimeMs)
    {
    }

    void init();
    void update(uint16_t lightLevel, uint32_t now);

    MotorState state() const;
    bool isPending() const;

private:
    MotorState requestedState(uint16_t lightLevel) const;

    Motor motor_;
    const uint16_t darkThreshold_;
    const uint16_t lightThreshold_;
    const uint32_t confirmTimeMs_;

    bool pending_ = false;
    uint32_t pendingSince_ = 0;
};
