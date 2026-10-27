#include "median_filter.h"

namespace lesson6 {
namespace task01 {

void MedianFilter::reset(uint16_t initialValue) {
    index_ = 0;

    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        readings_[i] = initialValue;
    }
}

uint16_t MedianFilter::update(uint16_t value) {
    readings_[index_] = value;
    index_ = (index_ + 1) % WINDOW_SIZE;

    uint16_t sorted[WINDOW_SIZE];

    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        sorted[i] = readings_[i];
    }

    for (uint8_t i = 1; i < WINDOW_SIZE; i++) {
        const uint16_t current = sorted[i];
        uint8_t position = i;

        while (position > 0 && sorted[position - 1] > current) {
            sorted[position] = sorted[position - 1];
            position--;
        }

        sorted[position] = current;
    }

    return sorted[WINDOW_SIZE / 2];
}

}
}
