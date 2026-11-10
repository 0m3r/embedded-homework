#pragma once

#include <stdint.h>

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
        uint8_t controlPin,
        uint8_t contactPin,
        uint8_t measurementCount,
        uint32_t debounceUs,
        uint32_t settleUs,
        uint32_t timeoutUs
    )
        : controlPin_(controlPin),
          contactPin_(contactPin),
          measurementCount_(measurementCount),
          debounceUs_(debounceUs),
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

    static void onContactChangeIsr();
    static volatile bool contactChanged_;
    static volatile uint32_t contactEdgeAtUs_;

    bool updateContact(uint32_t nowUs, bool& closed, uint32_t& changedAtUs);
    void processContact(bool closed, uint32_t changedAtUs, uint32_t nowUs);
    void setRelay(bool enabled, uint32_t nowUs);
    void storeMeasurement(RelayTransition transition, uint32_t durationUs);

    const uint8_t controlPin_;
    const uint8_t contactPin_;
    const uint8_t measurementCount_;
    const uint32_t debounceUs_;
    const uint32_t settleUs_;
    const uint32_t timeoutUs_;

    State state_ = State::SettlingBeforeOn;
    bool stableClosed_ = false;
    bool candidatePending_ = false;
    bool candidateClosed_ = false;
    uint32_t candidateAtUs_ = 0;
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
