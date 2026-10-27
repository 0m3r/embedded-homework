#include "moving_average.h"

namespace lesson6 {
namespace task01 {

void MovingAverage::reset(uint16_t initialValue) {
    sum_ = 0;
    index_ = 0;

    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        readings_[i] = initialValue;
        sum_ += initialValue;
    }
}

uint16_t MovingAverage::update(uint16_t value) {
    sum_ -= readings_[index_];
    sum_ += value;
    readings_[index_] = value;
    index_ = (index_ + 1) % WINDOW_SIZE;

    return sum_ / WINDOW_SIZE;
}

}
}
