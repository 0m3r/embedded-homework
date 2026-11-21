#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class Button {
public:
    struct Edge {
        std::int64_t timestampUs;
        bool pressed;
    };

    Button(gpio_num_t pin, bool activeHigh, bool internalPullDown);

    esp_err_t init();
    bool waitForEdge(Edge& edge, TickType_t timeout);
    bool isPressed() const;

private:
    static void IRAM_ATTR gpioInterrupt(void* context);

    const gpio_num_t pin_;
    const bool activeHigh_;
    const bool internalPullDown_;
    QueueHandle_t edgeQueue_ = nullptr;
};
