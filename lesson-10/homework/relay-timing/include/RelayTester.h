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

class RelayTester
{
public:
    constexpr RelayTester(
        Relay& relay,
        uint8_t measurementCount,
        uint32_t settleUs,
        uint32_t timeoutUs
    )
        : relay_(relay),
          measurementCount_(measurementCount),
          settleUs_(settleUs),
          timeoutUs_(timeoutUs)
    {
    }

    void init(uint32_t nowUs);
    void update(uint32_t nowUs);
    bool takeMeasurement(RelayMeasurement& measurement);

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

    void processContact(const RelayContactEvent& event, uint32_t nowUs);
    void setRelay(bool enabled, uint32_t nowUs);
    void storeMeasurement(RelayTransition transition, uint32_t durationUs);

    Relay& relay_;
    const uint8_t measurementCount_;
    const uint32_t settleUs_;
    const uint32_t timeoutUs_;

    State state_ = State::SettlingBeforeOn;
    uint32_t stateStartedAtUs_ = 0;
    uint32_t commandAtUs_ = 0;

    uint8_t onCount_ = 0;
    uint8_t offCount_ = 0;
    uint64_t onTotalUs_ = 0;
    uint64_t offTotalUs_ = 0;
    bool warmup_ = true;

    RelayMeasurement measurement_{};
    bool measurementReady_ = false;
};
