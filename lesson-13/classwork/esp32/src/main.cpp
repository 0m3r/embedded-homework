#include <Arduino.h>

#include "Application.hpp"

namespace {
Application application;
}

void setup()
{
    application.setup();
}

void loop()
{
    application.loop();
}
