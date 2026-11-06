#pragma once

#include <stdint.h>

// Перевір номери GPIO відповідно до свого підключення.
inline constexpr uint8_t LED_PIN = 16;
inline constexpr uint8_t LDR_PIN = 2;
inline constexpr uint8_t BUTTON_PIN = 18;

inline constexpr uint8_t LDR_FILTER_SIZE = 4;
inline constexpr uint8_t LDR_MAX_ZERO_READINGS = 5;

inline constexpr uint32_t LDR_PERIOD_MS = 10;
inline constexpr uint16_t LDR_DARK_THRESHOLD = 1800;
inline constexpr uint16_t LDR_LIGHT_THRESHOLD = 2200;

inline constexpr uint32_t BUTTON_POLL_PERIOD_MS = 10;
inline constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;
inline constexpr uint32_t BUTTON_LONG_PRESS_MS = 1000;

inline constexpr uint32_t LOG_PERIOD_MS = 500;

static_assert(LDR_FILTER_SIZE > 0, "LDR_FILTER_SIZE must be greater than zero");
static_assert(
    LDR_DARK_THRESHOLD < LDR_LIGHT_THRESHOLD,
    "LDR_DARK_THRESHOLD must be less than LDR_LIGHT_THRESHOLD"
);
