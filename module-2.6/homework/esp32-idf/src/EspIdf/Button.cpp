#include "EspIdf/Button.hpp"

#include "esp_attr.h"

Button::Button(gpio_num_t pin, std::uint32_t debounceMs)
    : pin_(pin),
      debounceUs_(static_cast<std::uint64_t>(debounceMs) * 1'000)
{
}

esp_err_t Button::init(PressCallback callback, void* context)
{
    if (callback == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    callback_ = callback;
    callbackContext_ = context;

    edgeQueue_ = xQueueCreate(16, sizeof(std::uint8_t));
    if (edgeQueue_ == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    esp_timer_create_args_t timerArgs = {};
    timerArgs.callback = timerCallback;
    timerArgs.arg = this;
    timerArgs.dispatch_method = ESP_TIMER_TASK;
    timerArgs.name = "button_debounce";

    esp_err_t result = esp_timer_create(&timerArgs, &timer_);
    if (result != ESP_OK) {
        return result;
    }

    gpio_config_t gpio = {};
    gpio.pin_bit_mask = 1ULL << pin_;
    gpio.mode = GPIO_MODE_INPUT;
    gpio.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio.intr_type = GPIO_INTR_ANYEDGE;

    result = gpio_config(&gpio);
    if (result != ESP_OK) {
        return result;
    }

    result = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    result = gpio_isr_handler_add(pin_, gpioInterrupt, this);
    if (result != ESP_OK) {
        return result;
    }

    const BaseType_t taskCreated = xTaskCreate(
        debounceTask,
        "button_debounce",
        3'072,
        this,
        10,
        nullptr);

    return taskCreated == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}

void IRAM_ATTR Button::gpioInterrupt(void* context)
{
    auto* button = static_cast<Button*>(context);
    const std::uint8_t event = 1;
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(
        button->edgeQueue_, &event, &higherPriorityTaskWoken);

    if (higherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void Button::debounceTask(void* context)
{
    static_cast<Button*>(context)->runDebounceTask();
}

void Button::timerCallback(void* context)
{
    static_cast<Button*>(context)->checkStableState();
}

void Button::runDebounceTask()
{
    std::uint8_t event;

    while (true) {
        xQueueReceive(edgeQueue_, &event, portMAX_DELAY);

        while (xQueueReceive(edgeQueue_, &event, 0) == pdTRUE) {
        }

        const esp_err_t stopResult = esp_timer_stop(timer_);
        if (stopResult == ESP_OK ||
            stopResult == ESP_ERR_INVALID_STATE) {
            esp_timer_start_once(timer_, debounceUs_);
        }
    }
}

void Button::checkStableState()
{
    const bool pressed = gpio_get_level(pin_) == 0;

    if (pressed && !stablePressed_) {
        callback_(callbackContext_);
    }

    stablePressed_ = pressed;
}
