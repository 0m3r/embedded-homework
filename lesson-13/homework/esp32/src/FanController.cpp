#include "FanController.hpp"

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

bool FanController::begin()
{
    pinMode(pin_, OUTPUT);
    write(false);

    return timer_.init(timerCallback, this) &&
           timer_.start(idleDurationMs_);
}

FanController::Event FanController::takeEvent()
{
    portENTER_CRITICAL(&mux_);
    const auto event = pendingEvent_;
    pendingEvent_ = Event::None;
    portEXIT_CRITICAL(&mux_);
    return event;
}

void ARDUINO_ISR_ATTR FanController::timerCallback(void* context)
{
    static_cast<FanController*>(context)->onTimeout();
}

void ARDUINO_ISR_ATTR FanController::onTimeout()
{
    toggle();

    portENTER_CRITICAL_ISR(&mux_);
    pendingEvent_ = running_ ? Event::Started : Event::Stopped;
    portEXIT_CRITICAL_ISR(&mux_);

    timer_.start(running_ ? runDurationMs_ : idleDurationMs_);
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
