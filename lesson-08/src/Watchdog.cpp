#include "Watchdog.h"

#include <Arduino.h>
#include <esp_err.h>
#include <esp_system.h>
#include <esp_task_wdt.h>

void Watchdog::init(uint32_t timeoutMs)
{
    esp_task_wdt_config_t config = {};
    config.timeout_ms = timeoutMs;
    config.idle_core_mask = 0;
    config.trigger_panic = true;

    esp_err_t result = esp_task_wdt_init(&config);

    // Arduino framework може активувати watchdog до setup().
    if (result == ESP_ERR_INVALID_STATE)
    {
        result = esp_task_wdt_reconfigure(&config);
    }

    ESP_ERROR_CHECK(result);

    // Підписуємо поточну loop-задачу, якщо вона ще не контролюється.
    if (esp_task_wdt_status(nullptr) != ESP_OK)
    {
        ESP_ERROR_CHECK(esp_task_wdt_add(nullptr));
    }

    Serial.printf(
        "Watchdog initialized: %lu ms\n",
        static_cast<unsigned long>(timeoutMs)
    );
    Serial.printf(
        "Reset reason: %d\n",
        static_cast<int>(esp_reset_reason())
    );
}

void Watchdog::feed()
{
    ESP_ERROR_CHECK(esp_task_wdt_reset());
}
