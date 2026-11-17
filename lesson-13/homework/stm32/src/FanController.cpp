#include "FanController.hpp"

#include <limits>

FanController::FanController(GPIO_TypeDef* port,
                             std::uint16_t pin,
                             bool activeHigh,
                             std::uint32_t runDurationMs,
                             std::uint32_t idleDurationMs)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh),
      runDurationMs_(runDurationMs),
      idleDurationMs_(idleDurationMs)
{
}

void FanController::begin()
{
    write(false);
}

std::uint32_t FanController::onTimeout()
{
    const bool wasRunning = running_;
    toggle();

    if (wasRunning) {
        if (pendingStops_ < std::numeric_limits<std::uint32_t>::max()) {
            pendingStops_ = pendingStops_ + 1U;
        }
    } else if (pendingStarts_ < std::numeric_limits<std::uint32_t>::max()) {
        pendingStarts_ = pendingStarts_ + 1U;
    }

    return running_ ? runDurationMs_ : idleDurationMs_;
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
    if (!running_) {
        write(true);
    }
}

void FanController::off()
{
    if (running_) {
        write(false);
    }
}

void FanController::toggle()
{
    running_ ? off() : on();
}

void FanController::write(bool enabled)
{
    running_ = enabled;
    const bool high = enabled == activeHigh_;
    HAL_GPIO_WritePin(port_, pin_, high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
