#include "EspIdf/TrafficLight.hpp"

#include "EspIdf/Config.hpp"
#include "esp_log.h"

namespace {
constexpr char Tag[] = "traffic_light";
}

TrafficLight::TrafficLight(
    gpio_num_t redPin,
    gpio_num_t yellowPin,
    gpio_num_t greenPin)
    : red_(redPin),
      yellow_(yellowPin),
      green_(greenPin)
{
}

esp_err_t TrafficLight::init()
{
    esp_err_t result = red_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = yellow_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = green_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = timer_.init(timerCallback, this);
    if (result != ESP_OK) {
        return result;
    }

    enter(State::Green);
    return ESP_OK;
}

void TrafficLight::update()
{
    if (timeout_.exchange(false)) {
        next();
    }
}

void TrafficLight::toggleSpecialMode()
{
    specialMode_ = !specialMode_;
    enter(specialMode_ ? State::FlashingYellowOn : State::Red);
}

void TrafficLight::timerCallback(void* context)
{
    auto* trafficLight = static_cast<TrafficLight*>(context);
    trafficLight->timeout_.store(true);
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
        if (greenFlashStep_ >= Config::GreenFlashSteps) {
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
    timeout_.store(false);
    state_ = state;

    switch (state_) {
    case State::Green:
        setLights(false, false, true);
        timer_.start(Config::GreenMs);
        break;
    case State::GreenFlashingOn:
        setLights(false, false, true);
        timer_.start(Config::GreenFlashHalfPeriodMs);
        break;
    case State::GreenFlashingOff:
        setLights(false, false, false);
        timer_.start(Config::GreenFlashHalfPeriodMs);
        break;
    case State::Yellow:
        setLights(false, true, false);
        timer_.start(Config::YellowMs);
        break;
    case State::Red:
        setLights(true, false, false);
        timer_.start(Config::RedMs);
        break;
    case State::RedYellow:
        setLights(true, true, false);
        timer_.start(Config::RedYellowMs);
        break;
    case State::FlashingYellowOn:
        setLights(false, true, false);
        timer_.start(Config::SpecialFlashHalfPeriodMs);
        break;
    case State::FlashingYellowOff:
        setLights(false, false, false);
        timer_.start(Config::SpecialFlashHalfPeriodMs);
        break;
    }

    ESP_LOGI(Tag, "State: %s", stateName());
}

void TrafficLight::setLights(bool red, bool yellow, bool green)
{
    red_.set(red);
    yellow_.set(yellow);
    green_.set(green);
}

const char* TrafficLight::stateName() const
{
    switch (state_) {
    case State::Green:
        return "Green";
    case State::GreenFlashingOn:
        return "Green flashing: ON";
    case State::GreenFlashingOff:
        return "Green flashing: OFF";
    case State::Yellow:
        return "Yellow";
    case State::Red:
        return "Red";
    case State::RedYellow:
        return "Red + Yellow";
    case State::FlashingYellowOn:
        return "Special yellow: ON";
    case State::FlashingYellowOff:
        return "Special yellow: OFF";
    }
    return "Unknown";
}
