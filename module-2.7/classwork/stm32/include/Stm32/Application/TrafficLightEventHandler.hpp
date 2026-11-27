#pragma once

#include "Application/IEventHandler.hpp"
#include "Application/TrafficLight.hpp"

class TrafficLightEventHandler final : public IEventHandler {
public:
    explicit TrafficLightEventHandler(TrafficLight& trafficLight);

    bool handles(EventType type) const override;
    void handle(const Event& event) override;

private:
    TrafficLight& trafficLight_;
};
