#include "Application/Event/TrafficLightEventHandler.hpp"

TrafficLightEventHandler::TrafficLightEventHandler(TrafficLight& trafficLight)
    : trafficLight_(trafficLight)
{
}

bool TrafficLightEventHandler::handles(EventType type) const
{
    switch (type) {
    case EventType::ButtonEdge:
    case EventType::DebounceTimeout:
    case EventType::TrafficTimeout:
        return true;
    }

    return false;
}

void TrafficLightEventHandler::handle(const Event& event)
{
    switch (event.type) {
    case EventType::ButtonEdge:
        trafficLight_.onButtonEdge();
        break;
    case EventType::DebounceTimeout:
        trafficLight_.onDebounceTimeout();
        break;
    case EventType::TrafficTimeout:
        trafficLight_.onTimeout();
        break;
    }
}
