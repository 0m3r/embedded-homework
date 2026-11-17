#include "FanController.hpp"

#include <limits>

FanController::FanController(std::uint8_t pin,
                             bool activeHigh,
                             std::uint32_t cyclePeriodSeconds,
                             std::uint32_t runDurationSeconds)
    : pin_(pin),
      activeHigh_(activeHigh),
      cyclePeriodSeconds_(cyclePeriodSeconds),
      runDurationSeconds_(runDurationSeconds)
{
}

void FanController::begin()
{
    pinMode(pin_, OUTPUT);
    off();
}

void ARDUINO_ISR_ATTR FanController::update()
{
    portENTER_CRITICAL_ISR(&mux_);
    elapsedSeconds_ = elapsedSeconds_ + 1U;

    if (running_ && elapsedSeconds_ >= runDurationSeconds_) {
        off();

        if (pendingStops_ < std::numeric_limits<std::uint32_t>::max()) {
            pendingStops_ = pendingStops_ + 1U;
        }
    } else if (!running_ && elapsedSeconds_ >= cyclePeriodSeconds_) {
        elapsedSeconds_ = 0;
        on();

        if (pendingStarts_ < std::numeric_limits<std::uint32_t>::max()) {
            pendingStarts_ = pendingStarts_ + 1U;
        }
    }

    portEXIT_CRITICAL_ISR(&mux_);
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
    write(true);
}

void ARDUINO_ISR_ATTR FanController::off()
{
    write(false);
}

void ARDUINO_ISR_ATTR FanController::toggle()
{
    write(!running_);
}

void ARDUINO_ISR_ATTR FanController::write(bool enabled)
{
    running_ = enabled;
    const bool high = enabled == activeHigh_;
    digitalWrite(pin_, high ? HIGH : LOW);
}
