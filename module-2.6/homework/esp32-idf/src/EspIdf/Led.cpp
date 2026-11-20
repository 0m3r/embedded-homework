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
        gpio_set_level(pin_, 0);
    }
    return result;
}

void Led::toggle()
{
    on_ = !on_;
    gpio_set_level(pin_, on_);
}

bool Led::isOn() const
{
    return on_;
}
