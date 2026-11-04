#include <Arduino.h>
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <esp_err.h>

#include "config.h"
#include "Button.h"
#include "Ldr.h"
#include "Led.h"

Button button;
Ldr ldr;
Led led;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    button.init();
    ldr.init();
    led.init();

    // Настройки watchdog для новой версии framework.
    esp_task_wdt_config_t wdtConfig = {};
    wdtConfig.timeout_ms = 5000;
    wdtConfig.idle_core_mask = 0;
    wdtConfig.trigger_panic = true;

    esp_err_t result = esp_task_wdt_init(&wdtConfig);
    //ESP_OK
    //ESP_ERR_INVALID_STATE

    // Framework мог уже включить watchdog при запуске.
    if (result == ESP_ERR_INVALID_STATE)
    {
        result = esp_task_wdt_reconfigure(&wdtConfig);
    }

    ESP_ERROR_CHECK(result);

    // Добавляем текущую задачу, если она ещё не подписана.
    if (esp_task_wdt_status(NULL) != ESP_OK)
    {
        ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    }

    Serial.println("Watchdog initialized: 5 seconds");

    Serial.printf(
        "reset reason: %d\n",
        esp_reset_reason()
    );
}

void loop()
{
    esp_task_wdt_reset();

    ldr.poll();
    button.poll();

    led.handler(button.get_state() && ldr.get_state());

    static uint32_t lastLog = 0;

    if (millis() - lastLog >= 200)
    {
        lastLog = millis();

        Serial.printf(
            "led cond %d %d\n",
            button.get_state(),
            ldr.get_state()
        );
    }

    delay(1);
}