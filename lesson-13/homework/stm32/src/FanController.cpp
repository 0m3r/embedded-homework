#include "FanController.hpp"

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

bool FanController::begin()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    write(false);

    GPIO_InitTypeDef gpio = {};
    gpio.Pin = pin_;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port_, &gpio);

    return timer_.init(timerCallback, this) &&
           timer_.start(idleDurationMs_);
}

FanController::Event FanController::takeEvent()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const auto event = pendingEvent_;
    pendingEvent_ = Event::None;

    if (interruptState == 0U) {
        __enable_irq();
    }
    return event;
}

void FanController::timerCallback(void* context)
{
    static_cast<FanController*>(context)->onTimeout();
}

void FanController::onTimeout()
{
    toggle();
    pendingEvent_ = running_ ? Event::Started : Event::Stopped;
    timer_.start(running_ ? runDurationMs_ : idleDurationMs_);
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
