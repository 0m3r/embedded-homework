#include "EspIdf/Button.hpp"

#include "esp_attr.h"
#include "esp_timer.h"

Button::Button(
    gpio_num_t pin,
    bool activeHigh,
    bool internalPullDown)
    : pin_(pin),
      activeHigh_(activeHigh),
      internalPullDown_(internalPullDown)
{
}

esp_err_t Button::init()
{
    edgeQueue_ = xQueueCreate(32, sizeof(Edge));
    if (edgeQueue_ == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    gpio_config_t gpio = {};
    gpio.pin_bit_mask = 1ULL << pin_;
    gpio.mode = GPIO_MODE_INPUT;
    gpio.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio.pull_down_en =
        internalPullDown_ ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
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
    const bool high = gpio_get_level(button->pin_) != 0;
    const Edge edge{
        esp_timer_get_time(),
        high == button->activeHigh_};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(
        button->edgeQueue_, &edge, &higherPriorityTaskWoken);

    if (higherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

bool Button::waitForEdge(Edge& edge, TickType_t timeout)
{
    return xQueueReceive(edgeQueue_, &edge, timeout) == pdTRUE;
}

bool Button::isPressed() const
{
    const bool high = gpio_get_level(pin_) != 0;
    return high == activeHigh_;
}
