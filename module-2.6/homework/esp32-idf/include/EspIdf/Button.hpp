#pragma once

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class Button {
public:
    explicit Button(gpio_num_t pin);

    esp_err_t init();
    bool waitForEdge(bool& pressed, TickType_t timeout);
    bool isPressed() const;

private:
    static void IRAM_ATTR gpioInterrupt(void* context);

    const gpio_num_t pin_;
    QueueHandle_t edgeQueue_ = nullptr;
};
