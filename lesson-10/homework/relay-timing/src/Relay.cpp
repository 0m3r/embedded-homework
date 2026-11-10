#include "Relay.h"

#include <Arduino.h>

void Relay::init()
{
    pinMode(controlPin_, OUTPUT);
    off();
}

void Relay::on()
{
    enabled_ = true;
    digitalWrite(controlPin_, HIGH);
}

void Relay::off()
{
    enabled_ = false;
    digitalWrite(controlPin_, LOW);
}

void Relay::toggle()
{
    if (enabled_)
    {
        off();
    }
    else
    {
        on();
    }
}
