#pragma once

#include <cstdint>

#include "Application/TrafficEvents.hpp"
#include "Core/Event/IEventHandler.hpp"
#include "Core/Input/DebouncedButton.hpp"
#include "Core/Output/LedFlasher.hpp"
#include "Hardware/ButtonInput.hpp"
#include "Hardware/Led.hpp"
#include "Hardware/OneShotTimer.hpp"
#include "Platform/Board.hpp"

class TrafficLight final : public IEventHandler {
public:
    void init();
    bool handles(EventType type) const override;
    void handle(const Event& event) override;

private:
    static constexpr std::uint32_t DebounceMs = 50;

    enum class State {
        Green,
        GreenFlashing,
        Yellow,
        Red,
        RedYellow,
        FlashingYellow,
    };

    void onButtonEdge();
    void onDebounceTimeout();
    void onTimeout();
    void next();
    void enter(State state);
    void setLights(bool red, bool yellow, bool green);
    void toggleSpecialMode();
    static void modeButtonPressed(void* context);

    ButtonInput modeButtonInput_{
        GPIOB,
        GPIO_PIN_0,
        true};
    OneShotTimer debounceTimer_{Board::debounceTimer()};
    DebouncedButton modeButton_{
        modeButtonInput_,
        debounceTimer_,
        DebounceMs};
    Led red_{GPIOC, GPIO_PIN_13, false};
    Led yellow_{GPIOC, GPIO_PIN_14, false};
    Led green_{GPIOC, GPIO_PIN_15, false};
    OneShotTimer timer_{Board::trafficTimer()};
    LedFlasher flasher_{timer_};
    State state_ = State::Red;
    bool specialMode_ = false;
};
