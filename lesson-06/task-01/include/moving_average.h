#pragma once

#include <stdint.h>

namespace lesson6 {
namespace task01 {

class MovingAverage {
public:
    static constexpr uint8_t WINDOW_SIZE = 4;

    void reset(uint16_t initialValue);
    uint16_t update(uint16_t value);

private:
    uint16_t readings_[WINDOW_SIZE] = {};
    uint32_t sum_ = 0;
    uint8_t index_ = 0;
};

}
}
