#pragma once

#include "EspIdf/ButtonExperiment.hpp"
#include "EspIdf/Config.hpp"
#include "esp_err.h"

class Application {
public:
    esp_err_t init();

private:
    static void task(void* context);
    void run();

    ButtonExperiment noRc_{
        "GPIO21 no RC",
        Config::ButtonNoRcPin,
        true,
        Config::LedNoRcPin,
        Config::DebounceMs};
    ButtonExperiment hardwareRc_{
        "GPIO18 hardware RC",
        Config::ButtonRcPin,
        false,
        Config::LedRcPin,
        Config::DebounceMs};
};
