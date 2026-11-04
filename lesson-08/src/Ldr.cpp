#include "Ldr.h"
#include "config.h"

#include <Arduino.h>

bool Ldr::get_state()
{
    return ldrState;
}

void Ldr::init()
{
    pinMode(LDR, INPUT);

    filterInit(analogRead(LDR));
}

void Ldr::poll()
{
    if (millis() - lastTimeChangeLdr > LDR_PERIOD_MS &&
        err_count < 5)
    {
        lastTimeChangeLdr = millis();

        uint16_t adc_value = analogRead(LDR);
        uint16_t filtered_adc_value = movingAvg(adc_value);

        // Учебная обработка нулевых показаний из урока.
        // После пяти таких показаний опрос прекращается.
        if (filtered_adc_value == 0)
        {
            err_count++;
            ldrState = true;
            return;
        }

        if (filtered_adc_value < LDR_THRESHOLD)
        {
            ldrState = true;

            Serial.printf(
                "adc raw value %d adc filtered %d errors %d\n",
                adc_value,
                filtered_adc_value,
                err_count
            );
        }
        else
        {
            ldrState = false;
        }
    }
}

void Ldr::filterInit(uint16_t initData)
{
    for (uint8_t i = 0; i < N; i++)
    {
        readings[i] = initData;
        sum += initData;
    }
}

// Восстановленная реализация скользящего среднего.
uint16_t Ldr::movingAvg(uint16_t rawData)
{
    sum -= readings[idx];

    readings[idx] = rawData;

    sum += readings[idx];

    idx = (idx + 1) % N;

    return static_cast<uint16_t>(sum / N);
}