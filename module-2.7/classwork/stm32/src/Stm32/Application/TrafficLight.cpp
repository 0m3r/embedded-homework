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

bool TrafficLight::handles(EventType type) const
{
    switch (type) {
    case EventType::ButtonEdge:
    case EventType::DebounceTimeout:
    case EventType::TrafficTimeout:
        return true;
    }

    return false;
}

void TrafficLight::handle(const Event& event)
{
    switch (event.type) {
    case EventType::ButtonEdge:
        onButtonEdge();
        break;
    case EventType::DebounceTimeout:
        onDebounceTimeout();
        break;
    case EventType::TrafficTimeout:
        onTimeout();
        break;
    }
}

void TrafficLight::onButtonEdge()
{
    modeButton_.onEdge();
}

void TrafficLight::onDebounceTimeout()
{
    modeButton_.onTimeout();
}

void TrafficLight::onTimeout()
{
    next();
}

void TrafficLight::toggleSpecialMode()
{
    specialMode_ = !specialMode_;
    enter(specialMode_ ? State::FlashingYellow : State::Red);
}

void TrafficLight::modeButtonPressed(void* context)
{
    static_cast<TrafficLight*>(context)->toggleSpecialMode();
}

void TrafficLight::next()
{
    switch (state_) {
    case State::Green:
        enter(State::GreenFlashing);
        break;
    case State::GreenFlashing:
        if (flasher_.onTimeout()) {
            enter(State::Yellow);
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
    case State::FlashingYellow:
        flasher_.onTimeout();
        break;
    }
}

void TrafficLight::enter(State state)
{
    flasher_.stop();
    state_ = state;

    switch (state_) {
    case State::Green:
        setLights(false, false, true);
        timer_.start(GreenMs);
        break;
    case State::GreenFlashing:
        setLights(false, false, false);
        flasher_.start(
            green_,
            GreenFlashHalfPeriodMs,
            GreenFlashSteps);
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
    case State::FlashingYellow:
        setLights(false, false, false);
        flasher_.start(
            yellow_,
            SpecialFlashHalfPeriodMs,
            LedFlasher::Continuous);
        break;
    }
}

void TrafficLight::setLights(bool red, bool yellow, bool green)
{
    red ? red_.on() : red_.off();
    yellow ? yellow_.on() : yellow_.off();
    green ? green_.on() : green_.off();
}
