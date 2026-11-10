#pragma once

#include <stdint.h>

struct RelayContactEvent
{
    bool closed;
    uint32_t changedAtUs;
};

class Relay
{
public:
    constexpr Relay(
        uint8_t controlPin,
        uint8_t feedbackPin,
        uint32_t debounceUs
    )
        : controlPin_(controlPin),
          feedbackPin_(feedbackPin),
          debounceUs_(debounceUs)
    {
    }

    void init();
    void setEnabled(bool enabled);
    bool tick(uint32_t nowUs, RelayContactEvent& event);

private:
    static void onContactChangeIsr();
    static volatile bool contactChanged_;
    static volatile uint32_t contactEdgeAtUs_;

    bool consumeInterrupt(uint32_t& edgeAtUs);

    const uint8_t controlPin_;
    const uint8_t feedbackPin_;
    const uint32_t debounceUs_;

    bool stableClosed_ = false;
    bool candidatePending_ = false;
    bool candidateClosed_ = false;
    uint32_t candidateAtUs_ = 0;
};
