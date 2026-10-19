#pragma once

#include <Arduino.h>
#include <initializer_list>

namespace led_controler {
constexpr uint8_t RED_PIN = 15;
constexpr uint8_t BLUE_PIN = 40;
constexpr uint8_t REVERSE_PIN = 4;

constexpr unsigned long POLICE_FLASH_MS = 120;
constexpr unsigned long POLICE_GAP_MS = 80;
constexpr unsigned long POLICE_GROUP_GAP_MS = 150;
constexpr unsigned long MODE_GAP_MS = 300;

void initialize();

void flashLeds(std::initializer_list<uint8_t> pins,
               unsigned long duration = POLICE_FLASH_MS,
               unsigned long gap = POLICE_GAP_MS);

void flashLeds(uint8_t pin, unsigned long duration = POLICE_FLASH_MS,
               unsigned long gap = POLICE_GAP_MS);

void startReverseTask();
}
