#include "Ldr.h"
#include "config.h"

#include <Arduino.h>

bool Ldr::isDark() const
{
    return ldrState;
}

bool Ldr::hasError() const
{
    return sensorError;
}

uint16_t Ldr::getRawValue() const
{
    return rawValue;
}

uint16_t Ldr::getFilteredValue() const
{
    return filteredValue;
}

void Ldr::init()
{
    pinMode(LDR_PIN, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(LDR_PIN, ADC_11db);

    rawValue = analogRead(LDR_PIN);
    filteredValue = rawValue;
    filterInit(rawValue);
}

void Ldr::poll()
{
    const uint32_t now = millis();

    if (now - lastTimeChangeLdr < LDR_PERIOD_MS)
    {
        return;
    }

    lastTimeChangeLdr = now;
    rawValue = analogRead(LDR_PIN);
    filteredValue = movingAvg(rawValue);

    if (filteredValue == 0)
    {
        if (err_count < LDR_MAX_ZERO_READINGS)
        {
            ++err_count;

            if (err_count == LDR_MAX_ZERO_READINGS)
            {
                Serial.println("LDR error: repeated zero readings");
                sensorError = true;
            }
        }

        ldrState = false;
        return;
    }

    if (sensorError)
    {
        Serial.println("LDR recovered");
    }

    sensorError = false;
    err_count = 0;

    if (filteredValue < LDR_DARK_THRESHOLD)
    {
        ldrState = true;
    }
    else if (filteredValue > LDR_LIGHT_THRESHOLD)
    {
        ldrState = false;
    }
}

void Ldr::filterInit(uint16_t initData)
{
    sum = 0;

    for (uint8_t i = 0; i < LDR_FILTER_SIZE; ++i)
    {
        readings[i] = initData;
        sum += initData;
    }
}

uint16_t Ldr::movingAvg(uint16_t rawData)
{
    sum -= readings[idx];
    readings[idx] = rawData;
    sum += rawData;

    idx = (idx + 1) % LDR_FILTER_SIZE;

    return static_cast<uint16_t>(sum / LDR_FILTER_SIZE);
}
