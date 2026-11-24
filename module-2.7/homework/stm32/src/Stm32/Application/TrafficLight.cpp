#include "Application/TrafficLight.hpp"

namespace {
constexpr std::uint32_t GreenMs = 5'000;
constexpr std::uint32_t GreenFlashHalfPeriodMs = 500;
constexpr std::uint32_t GreenFlashSteps = 6;
constexpr std::uint32_t YellowMs = 2'000;
constexpr std::uint32_t RedMs = 5'000;
constexpr std::uint32_t RedYellowMs = 1'000;
constexpr std::uint32_t SpecialFlashHalfPeriodMs = 500;
}

void TrafficLight::init()
{
    modeButton_.init(modeButtonPressed, this);
    enter(State::Green);
}

void TrafficLight::update()
{
    modeButton_.update();

    if (timer_.takeElapsed()) {
        next();
    }
}

void TrafficLight::onButtonInterrupt(std::uint16_t pin)
{
    modeButton_.onInterrupt(pin);
}

void TrafficLight::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    modeButton_.onTimerInterrupt(timer);
    timer_.onInterrupt(timer);
}

void TrafficLight::toggleSpecialMode()
{
    specialMode_ = !specialMode_;
    enter(specialMode_ ? State::FlashingYellowOn : State::Red);
}

void TrafficLight::modeButtonPressed(void* context)
{
    static_cast<TrafficLight*>(context)->toggleSpecialMode();
}

void TrafficLight::next()
{
    if (specialMode_) {
        enter(
            state_ == State::FlashingYellowOn
                ? State::FlashingYellowOff
                : State::FlashingYellowOn);
        return;
    }

    switch (state_) {
    case State::Green:
        greenFlashStep_ = 1;
        enter(State::GreenFlashingOn);
        break;
    case State::GreenFlashingOn:
    case State::GreenFlashingOff:
        if (greenFlashStep_ >= GreenFlashSteps) {
            enter(State::Yellow);
        } else {
            ++greenFlashStep_;
            enter(
                state_ == State::GreenFlashingOn
                    ? State::GreenFlashingOff
                    : State::GreenFlashingOn);
        }
        break;
    case State::Yellow:
        enter(State::Red);
        break;
    case State::Red:
        enter(State::RedYellow);
        break;
    case State::RedYellow:
        enter(State::Green);
        break;
    case State::FlashingYellowOn:
    case State::FlashingYellowOff:
        enter(State::Red);
        break;
    }
}

void TrafficLight::enter(State state)
{
    state_ = state;

    switch (state_) {
    case State::Green:
        setLights(false, false, true);
        timer_.start(GreenMs);
        break;
    case State::GreenFlashingOn:
        setLights(false, false, true);
        timer_.start(GreenFlashHalfPeriodMs);
        break;
    case State::GreenFlashingOff:
        setLights(false, false, false);
        timer_.start(GreenFlashHalfPeriodMs);
        break;
    case State::Yellow:
        setLights(false, true, false);
        timer_.start(YellowMs);
        break;
    case State::Red:
        setLights(true, false, false);
        timer_.start(RedMs);
        break;
    case State::RedYellow:
        setLights(true, true, false);
        timer_.start(RedYellowMs);
        break;
    case State::FlashingYellowOn:
        setLights(false, true, false);
        timer_.start(SpecialFlashHalfPeriodMs);
        break;
    case State::FlashingYellowOff:
        setLights(false, false, false);
        timer_.start(SpecialFlashHalfPeriodMs);
        break;
    }
}

void TrafficLight::setLights(bool red, bool yellow, bool green)
{
    red ? red_.on() : red_.off();
    yellow ? yellow_.on() : yellow_.off();
    green ? green_.on() : green_.off();
}
