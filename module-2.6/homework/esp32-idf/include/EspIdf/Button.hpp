#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class Button {
public:
    using PressCallback = void (*)(void* context);

    Button(gpio_num_t pin, std::uint32_t debounceMs);

    esp_err_t init(PressCallback callback, void* context);

private:
    static void IRAM_ATTR gpioInterrupt(void* context);
    static void debounceTask(void* context);
    static void timerCallback(void* context);

    void runDebounceTask();
    void checkStableState();

    const gpio_num_t pin_;
    const std::uint64_t debounceUs_;

    QueueHandle_t edgeQueue_ = nullptr;
    esp_timer_handle_t timer_ = nullptr;
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    bool stablePressed_ = false;
};
