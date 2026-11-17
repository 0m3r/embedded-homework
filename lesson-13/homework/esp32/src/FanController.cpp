#include "FanController.hpp"

#include <limits>

FanController::FanController(std::uint8_t pin,
                             bool activeHigh,
                             std::uint32_t runDurationMs,
                             std::uint32_t idleDurationMs)
    : pin_(pin),
      activeHigh_(activeHigh),
      runDurationMs_(runDurationMs),
      idleDurationMs_(idleDurationMs)
{
}

void FanController::begin()
{
    pinMode(pin_, OUTPUT);
    write(false);
}

std::uint32_t ARDUINO_ISR_ATTR FanController::onTimeout()
{
    portENTER_CRITICAL_ISR(&mux_);
    const bool wasRunning = running_;
    toggle();

    if (wasRunning) {
        if (pendingStops_ < std::numeric_limits<std::uint32_t>::max()) {
            pendingStops_ = pendingStops_ + 1U;
        }
    } else if (pendingStarts_ < std::numeric_limits<std::uint32_t>::max()) {
        pendingStarts_ = pendingStarts_ + 1U;
    }

    const auto nextIntervalMs =
        running_ ? runDurationMs_ : idleDurationMs_;
    portEXIT_CRITICAL_ISR(&mux_);
    return nextIntervalMs;
}

FanController::Events FanController::takeEvents()
{
    portENTER_CRITICAL(&mux_);
    const Events events{pendingStarts_, pendingStops_};
    pendingStarts_ = 0;
    pendingStops_ = 0;
    portEXIT_CRITICAL(&mux_);
    return events;
}

void ARDUINO_ISR_ATTR FanController::on()
{
    if (!running_) {
        write(true);
    }
}

void ARDUINO_ISR_ATTR FanController::off()
{
    if (running_) {
        write(false);
    }
}

void ARDUINO_ISR_ATTR FanController::toggle()
{
    running_ ? off() : on();
}

void ARDUINO_ISR_ATTR FanController::write(bool enabled)
{
    running_ = enabled;
    const bool high = enabled == activeHigh_;
    digitalWrite(pin_, high ? HIGH : LOW);
}
