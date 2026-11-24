#include "EspIdf/Application.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char Tag[] = "traffic_light";
}

esp_err_t Application::init()
{
    esp_err_t result = modeButton_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = trafficLight_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = modeButtonDebouncer_.init(modeButtonPressed, this);
    if (result != ESP_OK) {
        return result;
    }

    const BaseType_t created =
        xTaskCreate(task, "traffic_light", 3'072, this, 10, nullptr);
    if (created != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(Tag, "Ready: red=GPIO12, yellow=GPIO16, green=GPIO17");
    return ESP_OK;
}

void Application::task(void* context)
{
    static_cast<Application*>(context)->run();
}

void Application::modeButtonPressed(void* context)
{
    auto* application = static_cast<Application*>(context);
    application->modeToggleRequested_.store(true);
}

void Application::run()
{
    const TickType_t pollDelayTicks =
        pdMS_TO_TICKS(Config::PollIntervalMs) > 0
            ? pdMS_TO_TICKS(Config::PollIntervalMs)
            : 1;

    while (true) {
        Button::Edge edge{};
        while (modeButton_.waitForEdge(edge, 0)) {
            modeButtonDebouncer_.onEdge(edge.pressed);
        }

        if (modeToggleRequested_.exchange(false)) {
            trafficLight_.toggleSpecialMode();
        }

        trafficLight_.update();
        vTaskDelay(pollDelayTicks);
    }
}
