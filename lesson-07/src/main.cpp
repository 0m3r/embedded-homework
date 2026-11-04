#include <Arduino.h>

constexpr uint8_t LDR_PIN = 2;
constexpr uint8_t RELAY_PIN = 15;

constexpr int THRESHOLD_DARK = 1500;
constexpr int THRESHOLD_LIGHT = 2500;

bool relayOn = false;

void setup()
{
    Serial.begin(115200);

    pinMode(LDR_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);

    digitalWrite(RELAY_PIN, LOW);
    analogReadResolution(12);
}

void loop()
{
    int adcValue = analogRead(LDR_PIN);

    if (adcValue < THRESHOLD_DARK)
    {
        relayOn = true;
    }
    else if (adcValue > THRESHOLD_LIGHT)
    {
        relayOn = false;
    }

    digitalWrite(RELAY_PIN, relayOn ? HIGH : LOW);

    Serial.printf(
        "ADC: %d | Relay: %s\n",
        adcValue,
        relayOn ? "ON" : "OFF"
    );

    delay(500);
}
