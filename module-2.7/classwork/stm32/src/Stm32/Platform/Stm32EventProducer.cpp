#include "Platform/Stm32EventProducer.hpp"

#include "Platform/Board.hpp"

Stm32EventProducer::Stm32EventProducer(IEventDispatcher& dispatcher)
    : dispatcher_(dispatcher)
{
}

void Stm32EventProducer::onButtonInterrupt(std::uint16_t pin)
{
    if (pin == GPIO_PIN_0) {
        dispatcher_.post(Event{TrafficEvent::ButtonEdge, HAL_GetTick()});
    }
}

void Stm32EventProducer::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    if (timer == &Board::debounceTimer()) {
        dispatcher_.post(Event{TrafficEvent::DebounceTimeout, HAL_GetTick()});
    } else if (timer == &Board::trafficTimer()) {
        dispatcher_.post(Event{TrafficEvent::TrafficTimeout, HAL_GetTick()});
    }
}
