#include "EspIdf/Application.hpp"

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char Tag[] = "debounce";
}

esp_err_t Application::init()
{
    esp_err_t result = led_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = button_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = timerFsm_.init(buttonPressed, this);
    if (result != ESP_OK) {
        return result;
    }

    const BaseType_t created =
        xTaskCreate(task, "debounce", 3'072, this, 10, nullptr);
    if (created != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(Tag, "Ready: GPIO21 button -> GPIO15 LED");
    return ESP_OK;
}

void Application::task(void* context)
{
    static_cast<Application*>(context)->run();
}

void Application::buttonPressed(void* context)
{
    static_cast<Application*>(context)->onButtonPressed();
}

void Application::run()
{
    while (true) {
        bool pressed = false;
        const bool hasEdge = button_.waitForEdge(
            pressed, pdMS_TO_TICKS(Config::PollIntervalMs));
        const auto nowMs =
            static_cast<std::uint64_t>(esp_timer_get_time() / 1'000);

        if (hasEdge) {
            if (noDebouncer_.onEdge(pressed)) {
                ESP_LOGI(
                    Tag, "No debounce: %lu",
                    static_cast<unsigned long>(
                        noDebouncer_.pressCount()));
            }

            if (timeDebouncer_.onEdge(pressed, nowMs)) {
                ESP_LOGI(
                    Tag, "Time-based: %lu",
                    static_cast<unsigned long>(
                        timeDebouncer_.pressCount()));
            }

            timerFsm_.onEdge(pressed);
        }

        if (pollingFsm_.update(button_.isPressed(), nowMs)) {
            ESP_LOGI(
                Tag, "Polling FSM: %lu",
                static_cast<unsigned long>(
                    pollingFsm_.pressCount()));
        }
    }
}

void Application::onButtonPressed()
{
    led_.toggle();
    ESP_LOGI(
        Tag, "Timer FSM: %lu, LED %s",
        static_cast<unsigned long>(timerFsm_.pressCount()),
        led_.isOn() ? "ON" : "OFF");
}
