#include "EspIdf/Led.hpp"

Led::Led(gpio_num_t pin)
    : pin_(pin)
{
}

esp_err_t Led::init()
{
    gpio_config_t config = {};
    config.pin_bit_mask = 1ULL << pin_;
    config.mode = GPIO_MODE_OUTPUT;

    const esp_err_t result = gpio_config(&config);
    if (result == ESP_OK) {
        set(false);
    }
    return result;
}

void Led::set(bool on)
{
    gpio_set_level(pin_, on);
}
