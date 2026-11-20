#include "EspIdf/Application.hpp"

#include "esp_log.h"

namespace {
constexpr char Tag[] = "debounce";
}

esp_err_t Application::init()
{
    esp_err_t result = led_.init();
    if (result != ESP_OK) {
        return result;
    }

    result = button_.init(buttonPressed, this);
    if (result == ESP_OK) {
        ESP_LOGI(Tag, "Ready: GPIO21 button -> GPIO15 LED");
    }
    return result;
}

void Application::buttonPressed(void* context)
{
    static_cast<Application*>(context)->onButtonPressed();
}

void Application::onButtonPressed()
{
    led_.toggle();
    ESP_LOGI(Tag, "LED %s", led_.isOn() ? "ON" : "OFF");
}
