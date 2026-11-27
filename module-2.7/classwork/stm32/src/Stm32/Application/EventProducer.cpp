#include "Application/EventProducer.hpp"

#include "Platform/Board.hpp"

EventProducer::EventProducer(EventDispatcher& dispatcher)
    : dispatcher_(dispatcher)
{
}

void EventProducer::onButtonInterrupt(std::uint16_t pin)
{
    if (pin == GPIO_PIN_0) {
        dispatcher_.post(Event{EventType::ButtonEdge, HAL_GetTick()});
    }
}

void EventProducer::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    if (timer == &Board::debounceTimer()) {
        dispatcher_.post(Event{EventType::DebounceTimeout, HAL_GetTick()});
    } else if (timer == &Board::trafficTimer()) {
        dispatcher_.post(Event{EventType::TrafficTimeout, HAL_GetTick()});
    }
}
