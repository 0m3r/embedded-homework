#include "EspIdf/ButtonExperiment.hpp"

#include "esp_log.h"

namespace {
constexpr char Tag[] = "debounce";
}

ButtonExperiment::ButtonExperiment(
    const char* name,
    gpio_num_t buttonPin,
    bool internalPullDown,
    gpio_num_t ledPin,
    std::uint32_t debounceMs)
    : name_(name),
      button_(buttonPin, true, internalPullDown),
      led_(ledPin),
      timeDebouncer_(debounceMs),
      timerFsm_(button_, debounceMs),
      pollingFsm_(debounceMs)
{
}

esp_err_t ButtonExperiment::init()
{
    esp_err_t result = led_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = button_.init();
    if (result != ESP_OK) {
        return result;
    }

    return timerFsm_.init(timerFsmPressed, this);
}

void ButtonExperiment::update(std::uint64_t nowMs)
{
    Button::Edge edge{};

    while (button_.waitForEdge(edge, 0)) {
        logEdge(edge);

        if (noDebouncer_.onEdge(edge.pressed)) {
            log("No debounce", noDebouncer_.pressCount());
        }

        const auto edgeMs =
            static_cast<std::uint64_t>(edge.timestampUs / 1'000);
        if (timeDebouncer_.onEdge(edge.pressed, edgeMs)) {
            log("Time-based", timeDebouncer_.pressCount());
        }

        timerFsm_.onEdge(edge.pressed);
    }

    if (pollingFsm_.update(button_.isPressed(), nowMs)) {
        log("Polling FSM", pollingFsm_.pressCount());
    }
}

void ButtonExperiment::timerFsmPressed(void* context)
{
    static_cast<ButtonExperiment*>(context)->onTimerFsmPressed();
}

void ButtonExperiment::onTimerFsmPressed()
{
    led_.toggle();
    log("Timer FSM", timerFsm_.pressCount());
}

void ButtonExperiment::logEdge(const Button::Edge& edge) const
{
    ESP_LOGI(
        Tag,
        "WAVE,%s,%lld,%u",
        name_,
        static_cast<long long>(edge.timestampUs),
        edge.pressed ? 1U : 0U);
}

void ButtonExperiment::log(
    const char* method,
    std::uint32_t count) const
{
    ESP_LOGI(
        Tag,
        "[%s] %s: %lu",
        name_,
        method,
        static_cast<unsigned long>(count));
}
