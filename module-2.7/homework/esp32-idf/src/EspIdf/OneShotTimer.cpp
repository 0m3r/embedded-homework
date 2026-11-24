#include "EspIdf/OneShotTimer.hpp"

#include "esp_log.h"

namespace {
constexpr char Tag[] = "one_shot_timer";
}

OneShotTimer::OneShotTimer(const char* name)
    : name_(name)
{
}

esp_err_t OneShotTimer::init(
    Callback callback,
    void* context)
{
    if (callback == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_timer_create_args_t args = {};
    args.callback = callback;
    args.arg = context;
    args.name = name_;
    return esp_timer_create(&args, &timer_);
}

void OneShotTimer::start(std::uint32_t durationMs)
{
    const esp_err_t stopResult = esp_timer_stop(timer_);
    if (stopResult != ESP_OK &&
        stopResult != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(
            Tag,
            "%s stop failed: %s",
            name_,
            esp_err_to_name(stopResult));
        return;
    }

    const esp_err_t startResult = esp_timer_start_once(
        timer_,
        static_cast<std::uint64_t>(durationMs) * 1'000);
    if (startResult != ESP_OK) {
        ESP_LOGE(
            Tag,
            "%s start failed: %s",
            name_,
            esp_err_to_name(startResult));
    }
}
