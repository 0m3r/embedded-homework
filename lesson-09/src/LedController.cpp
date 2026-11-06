#include "LedController.h"

void LedController::init(uint32_t now)
{
    led_.init();
    setLastBlinkAt(now);
    apply();
}

void LedController::update(uint32_t now)
{
    if (mode_ == LedMode::Blink)
    {
        blink(now);
    }
}

void LedController::next(uint32_t now)
{
    mode_ = getNextMode(mode_);
    setLastBlinkAt(now);
    apply();
}

void LedController::setLastBlinkAt(uint32_t now)
{
    lastBlinkAt_ = now;
}

LedMode LedController::mode() const
{
    return mode_;
}

void LedController::apply()
{
    switch (mode_)
    {
        case LedMode::Blink:
        case LedMode::AlwaysOff:
            off();
            break;

        case LedMode::AlwaysOn:
            on();
            break;
    }
}

void LedController::blink(uint32_t now)
{
    if (now - lastBlinkAt_ < blinkIntervalMs_)
    {
        return;
    }

    setLastBlinkAt(now);
    toggle();
}

void LedController::on()
{
    led_.set(LedState::On);
}

void LedController::off()
{
    led_.set(LedState::Off);
}

void LedController::toggle()
{
    led_.set(
        led_.state() == LedState::On
            ? LedState::Off
            : LedState::On
    );
}
