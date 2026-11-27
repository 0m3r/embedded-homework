#pragma once

#include <cstdint>

#include "Application/Event/IEventHandler.hpp"
#include "Hardware/Button.hpp"
#include "Hardware/Led.hpp"
#include "Hardware/LedFlasher.hpp"
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

    Button modeButton_{
        GPIOB,
        GPIO_PIN_0,
        true,
        Board::debounceTimer(),
        DebounceMs};
    Led red_{GPIOC, GPIO_PIN_13, false};
    Led yellow_{GPIOC, GPIO_PIN_14, false};
    Led green_{GPIOC, GPIO_PIN_15, false};
    OneShotTimer timer_{Board::trafficTimer()};
    LedFlasher flasher_{timer_};
    State state_ = State::Red;
    bool specialMode_ = false;
};
