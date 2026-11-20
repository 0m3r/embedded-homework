#pragma once

#include <cstdint>

class Led {
public:
    explicit Led(std::uint8_t pin);

    void init();
    void toggle();
    bool isOn() const;

private:
    const std::uint8_t pin_;
    bool on_ = false;
};
