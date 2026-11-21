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
    esp_err_t result = noRc_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = hardwareRc_.init();
    if (result != ESP_OK) {
        return result;
    }

    const BaseType_t created =
        xTaskCreate(task, "debounce", 3'072, this, 10, nullptr);
    if (created != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(
        Tag,
        "Ready: GPIO21->GPIO12, GPIO18 RC->GPIO16");
    return ESP_OK;
}

void Application::task(void* context)
{
    static_cast<Application*>(context)->run();
}

void Application::run()
{
    const TickType_t pollDelayTicks =
        pdMS_TO_TICKS(Config::PollIntervalMs) > 0
            ? pdMS_TO_TICKS(Config::PollIntervalMs)
            : 1;

    while (true) {
        const auto nowMs =
            static_cast<std::uint64_t>(esp_timer_get_time() / 1'000);

        noRc_.update(nowMs);
        hardwareRc_.update(nowMs);
        vTaskDelay(pollDelayTicks);
    }
}
