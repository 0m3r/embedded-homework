#include "EspIdf/Application.hpp"

#include "esp_check.h"

extern "C" void app_main()
{
    static Application application;
    ESP_ERROR_CHECK(application.init());
}
