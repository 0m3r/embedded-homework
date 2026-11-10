#include <Arduino.h>

#include "Application.h"

Application& getApplication()
{
    static Application application;
    return application;
}

void setup()
{
    getApplication().setup();
}

void loop()
{
    getApplication().loop();
}
