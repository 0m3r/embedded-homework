#pragma once

#include <stdint.h>

#include "Relay.h"

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

struct RelayTestConfig
{
    uint8_t measurementCount;
    uint32_t settleUs;
    uint32_t timeoutUs;
};

struct RelayStatistics
{
    uint8_t completedCycles = 0;
    uint64_t onTotalUs = 0;
    uint64_t offTotalUs = 0;
};

class RelayTester
{
public:
    constexpr RelayTester(Relay& relay, RelayTestConfig config)
        : relay_(relay),
          config_(config)
    {
    }

    void init(uint32_t nowUs);
    bool update(uint32_t nowUs, RelayMeasurement& measurement);

    bool complete() const;
    bool failed() const;
    uint32_t averageOnUs() const;
    uint32_t averageOffUs() const;

private:
    enum class State : uint8_t
    {
        SettlingBeforeOn,
        WaitingForOn,
        HoldingOn,
        WaitingForOff,
        HoldingOff,
        Complete,
        Failed
    };

    bool processContact(
        const RelayContactEvent& event,
        uint32_t nowUs,
        RelayMeasurement& measurement
    );
    void setRelay(bool enabled, uint32_t nowUs);
    void recordMeasurement(
        RelayTransition transition,
        uint32_t durationUs,
        RelayMeasurement& measurement
    );

    Relay& relay_;
    const RelayTestConfig config_;
    RelayStatistics statistics_;

    State state_ = State::SettlingBeforeOn;
    uint32_t stateStartedAtUs_ = 0;
    uint32_t commandAtUs_ = 0;
    bool warmup_ = true;
};
