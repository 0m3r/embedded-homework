#include "Led.h"
#include "config.h"

#include <Arduino.h>

void Led::init()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void Led::set(bool enabled)
{
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
}
