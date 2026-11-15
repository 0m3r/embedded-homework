#include "Stm32/Application/ButtonExperiment.hpp"

ButtonExperiment::ButtonExperiment(DebugLogger& logger,
                                   const char* source,
                                   GPIO_TypeDef* buttonPort,
                                   std::uint16_t buttonPin,
                                   bool buttonActiveHigh,
                                   GPIO_TypeDef* ledPort,
                                   std::uint16_t ledPin,
                                   bool ledActiveLow,
                                   std::uint32_t debounceMs,
                                   std::uint32_t pollIntervalMs)
    : logger_(logger),
      source_(source),
      button_(buttonPort, buttonPin, buttonActiveHigh),
      led_(ledPort, ledPin, ledActiveLow),
      experiment_(*this, debounceMs, pollIntervalMs)
{
}

void ButtonExperiment::init()
{
    button_.init();
    led_.off();
}

void ButtonExperiment::update(std::uint32_t nowMs)
{
    const DebounceInput input{
        button_.takeInterrupts(), button_.isPressed(), nowMs};

    experiment_.update(input);

    if (experiment_.acceptedCount() != ledCount_) {
        ledCount_ = experiment_.acceptedCount();
        led_.toggle();
    }
}

void ButtonExperiment::press(const char* method, std::uint32_t count) const
{
    logger_.press(source_, method, count);
}

void ButtonExperiment::onInterrupt(std::uint16_t pin)
{
    button_.onInterrupt(pin);
}
