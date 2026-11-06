#include <Arduino.h>

#include "config.h"
#include "Button.h"
#include "Ldr.h"
#include "Led.h"
#include "Watchdog.h"

Button button;
Ldr ldr;
Led led;
Watchdog watchdog;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    button.init();
    ldr.init();
    led.init();

    watchdog.init(5000);
}

void loop()
{
    watchdog.feed();

    ldr.poll();
    button.poll();

    const bool longPressed = button.isLongPressed();
    const bool dark = ldr.isDark();
    const bool ledOn = longPressed && dark && !ldr.hasError();

    led.set(ledOn);

    static uint32_t lastLog = 0;

    if (millis() - lastLog >= LOG_PERIOD_MS)
    {
        lastLog = millis();

        Serial.printf(
            "ADC raw: %u | avg: %u | dark: %s | button: %s | LED: %s | error: %s\n",
            static_cast<unsigned>(ldr.getRawValue()),
            static_cast<unsigned>(ldr.getFilteredValue()),
            dark ? "YES" : "NO",
            longPressed ? "PRESSED" : "RELEASED",
            ledOn ? "ON" : "OFF",
            ldr.hasError() ? "YES" : "NO"
        );
    }

    delay(1);
}
