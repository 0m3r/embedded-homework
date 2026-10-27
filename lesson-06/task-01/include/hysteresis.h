#pragma once

#include <stdint.h>

namespace lesson6 {
namespace task01 {

class Hysteresis {
public:
    Hysteresis(uint16_t onThreshold, uint16_t offThreshold);

    bool update(uint16_t value);
    bool isOn() const;

private:
    uint16_t onThreshold_;
    uint16_t offThreshold_;
    bool isOn_ = false;
};

}
}
