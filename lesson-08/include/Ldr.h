#pragma once

#include <Arduino.h>
#include "config.h"

class Ldr
{
private:
    uint16_t readings[LDR_FILTER_SIZE] = {};
    uint8_t idx = 0;
    uint32_t sum = 0;

    bool ldrState = false;
    bool sensorError = false;

    uint32_t lastTimeChangeLdr = 0;
    uint8_t err_count = 0;
    uint16_t rawValue = 0;
    uint16_t filteredValue = 0;

    uint16_t movingAvg(uint16_t rawData);
    void filterInit(uint16_t initData);

public:
    void init();
    void poll();
    bool isDark() const;
    bool hasError() const;
    uint16_t getRawValue() const;
    uint16_t getFilteredValue() const;
};
