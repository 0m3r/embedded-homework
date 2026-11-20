#pragma once

#include "EspIdf/Button.hpp"
#include "EspIdf/Config.hpp"
#include "EspIdf/Led.hpp"
#include "esp_err.h"

class Application {
public:
    esp_err_t init();

private:
    static void buttonPressed(void* context);
    void onButtonPressed();

    Button button_{Config::ButtonPin, Config::DebounceMs};
    Led led_{Config::LedPin};
};
