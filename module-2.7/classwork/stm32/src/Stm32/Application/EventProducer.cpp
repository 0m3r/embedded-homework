#include "Application/EventProducer.hpp"

#include "Platform/Board.hpp"

EventProducer::EventProducer(EventDispatcher& dispatcher)
    : dispatcher_(dispatcher)
{
}

void EventProducer::onButtonInterrupt(std::uint16_t pin)
{
    if (pin == GPIO_PIN_0) {
        dispatcher_.post(EventType::ButtonEdge);
    }
}

void EventProducer::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    if (timer == &Board::debounceTimer()) {
        dispatcher_.post(EventType::DebounceTimeout);
    } else if (timer == &Board::trafficTimer()) {
        dispatcher_.post(EventType::TrafficTimeout);
    }
}
