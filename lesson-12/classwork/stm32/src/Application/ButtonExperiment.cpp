#include "Application/ButtonExperiment.hpp"

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
    : button_(buttonPort, buttonPin, buttonActiveHigh),
      led_(ledPort, ledPin, ledActiveLow),
      noDebounce_(logger, source),
      timeBased_(logger, source, debounceMs),
      stateBased_(logger, source, debounceMs),
      polling_(logger, source, pollIntervalMs, debounceMs)
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

    for (auto* debouncer : debouncers_) {
        debouncer->debounce(input);
    }

    if (stateBased_.count() != ledCount_) {
        ledCount_ = stateBased_.count();
        led_.toggle();
    }
}

void ButtonExperiment::onInterrupt(std::uint16_t pin)
{
    button_.onInterrupt(pin);
}
