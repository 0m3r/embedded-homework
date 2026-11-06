#include "LedController.h"

void LedController::init(uint32_t now)
{
    led_.init();
    lastBlinkAt_ = now;
    applyMode();
}

void LedController::update(uint32_t now)
{
    if (mode_ != LedMode::Blink ||
        now - lastBlinkAt_ < blinkIntervalMs_)
    {
        return;
    }

    lastBlinkAt_ = now;
    led_.set(
        led_.state() == LedState::On
            ? LedState::Off
            : LedState::On
    );
}

void LedController::selectNextMode(uint32_t now)
{
    mode_ = nextMode(mode_);
    lastBlinkAt_ = now;
    applyMode();
}

LedMode LedController::mode() const
{
    return mode_;
}

void LedController::applyMode()
{
    switch (mode_)
    {
        case LedMode::Blink:
        case LedMode::AlwaysOff:
            led_.set(LedState::Off);
            break;

        case LedMode::AlwaysOn:
            led_.set(LedState::On);
            break;
    }
}
