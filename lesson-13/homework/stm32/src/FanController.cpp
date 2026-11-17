#include "FanController.hpp"

#include <limits>

FanController::FanController(GPIO_TypeDef* port,
                             std::uint16_t pin,
                             bool activeHigh,
                             std::uint32_t cyclePeriodSeconds,
                             std::uint32_t runDurationSeconds)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh),
      cyclePeriodSeconds_(cyclePeriodSeconds),
      runDurationSeconds_(runDurationSeconds)
{
}

void FanController::begin()
{
    off();
}

void FanController::onPeriod()
{
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
}

FanController::Events FanController::takeEvents()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const Events events{pendingStarts_, pendingStops_};
    pendingStarts_ = 0;
    pendingStops_ = 0;

    if (interruptState == 0U) {
        __enable_irq();
    }
    return events;
}

void FanController::on()
{
    write(true);
}

void FanController::off()
{
    write(false);
}

void FanController::toggle()
{
    write(!running_);
}

void FanController::write(bool enabled)
{
    running_ = enabled;
    const bool high = enabled == activeHigh_;
    HAL_GPIO_WritePin(port_, pin_, high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
