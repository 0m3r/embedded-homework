#pragma once

#include <Arduino.h>

class Button
{
private:
    uint32_t lastTimeChangeBtn = 0; // час останньої перевірки натиску кнопки
    uint32_t btnTimekeeper = 0;  // час першого натиску
    bool buttonState = false;

public:
    void init();
    void poll();
    bool get_state();
};