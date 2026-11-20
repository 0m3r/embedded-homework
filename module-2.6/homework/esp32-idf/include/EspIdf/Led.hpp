#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

class Led {
public:
    explicit Led(gpio_num_t pin);

    esp_err_t init();
    void toggle();
    bool isOn() const;

private:
    const gpio_num_t pin_;
    bool on_ = false;
};
