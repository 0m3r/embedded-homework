#pragma once

#include <stdint.h>

#include "Relay.h"
#include "RelayContact.h"

enum class RelayTransition : uint8_t
{
    TurnOn,
    TurnOff
};

struct RelayMeasurement
{
    RelayTransition transition;
    uint8_t number;
    uint32_t durationUs;
};

class RelayTester
{
public:
    constexpr RelayTester(
        Relay& relay,
        RelayContact& contact,
        uint8_t measurementCount,
        uint32_t settleUs,
        uint32_t timeoutUs
    )
        : relay_(relay),
          contact_(contact),
          measurementCount_(measurementCount),
          settleUs_(settleUs),
          timeoutUs_(timeoutUs)
    {
    }

    void init(uint32_t nowUs);
    void restart(uint32_t nowUs);
    bool tick(uint32_t nowUs, RelayMeasurement& measurement);

    bool complete() const;
    bool failed() const;
    uint32_t averageOnUs() const;
    uint32_t averageOffUs() const;

private:
    enum class Phase : uint8_t
    {
        TurnOn,
        TurnOff,
        Complete,
        Failed
    };

    void startTransition(uint32_t nowUs);
    bool finishTransition(
        uint32_t nowUs,
        RelayMeasurement& measurement
    );

    Relay& relay_;
    RelayContact& contact_;
    const uint8_t measurementCount_;
    const uint32_t settleUs_;
    const uint32_t timeoutUs_;

    Phase phase_ = Phase::TurnOn;
    bool waiting_ = false;
    bool warmup_ = true;
    uint8_t cycle_ = 0;

    uint32_t commandAtUs_ = 0;
    uint32_t pauseAtUs_ = 0;
    uint64_t onTotalUs_ = 0;
    uint64_t offTotalUs_ = 0;
};
