#pragma once

#include <cstdint>

#include "esp_err.h"
#include "esp_timer.h"

class OneShotTimer {
public:
    using Callback = void (*)(void* context);

    explicit OneShotTimer(const char* name);

    esp_err_t init(Callback callback, void* context);
    void start(std::uint32_t durationMs);

private:
    const char* const name_;
    esp_timer_handle_t timer_ = nullptr;
};
