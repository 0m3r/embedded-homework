#include "hysteresis.h"

namespace lesson6 {
namespace task01 {

Hysteresis::Hysteresis(uint16_t onThreshold, uint16_t offThreshold)
    : onThreshold_(onThreshold), offThreshold_(offThreshold) {
}

bool Hysteresis::update(uint16_t value) {
    if (!isOn_ && value < onThreshold_) {
        isOn_ = true;
    } else if (isOn_ && value > offThreshold_) {
        isOn_ = false;
    }

    return isOn_;
}

bool Hysteresis::isOn() const {
    return isOn_;
}

}
}
