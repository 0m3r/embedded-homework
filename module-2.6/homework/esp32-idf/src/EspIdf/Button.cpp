#include "EspIdf/Button.hpp"

#include "esp_attr.h"

Button::Button(gpio_num_t pin)
    : pin_(pin)
{
}

esp_err_t Button::init()
{
    edgeQueue_ = xQueueCreate(32, sizeof(bool));
    if (edgeQueue_ == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    gpio_config_t gpio = {};
    gpio.pin_bit_mask = 1ULL << pin_;
    gpio.mode = GPIO_MODE_INPUT;
    gpio.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio.intr_type = GPIO_INTR_ANYEDGE;

    esp_err_t result = gpio_config(&gpio);
    if (result != ESP_OK) {
        return result;
    }

    result = gpio_install_isr_service(0);
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    return gpio_isr_handler_add(pin_, gpioInterrupt, this);
}

void IRAM_ATTR Button::gpioInterrupt(void* context)
{
    auto* button = static_cast<Button*>(context);
    const bool pressed = gpio_get_level(button->pin_) == 0;
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(
        button->edgeQueue_, &pressed, &higherPriorityTaskWoken);

    if (higherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

bool Button::waitForEdge(bool& pressed, TickType_t timeout)
{
    return xQueueReceive(edgeQueue_, &pressed, timeout) == pdTRUE;
}

bool Button::isPressed() const
{
    return gpio_get_level(pin_) == 0;
}
