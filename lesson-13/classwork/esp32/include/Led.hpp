#pragma once

#include <cstdint>

class Led {
public:
    explicit Led(std::uint8_t pin);

    void begin();
    void off();
    void toggle();

private:
    void write(bool enabled);

    const std::uint8_t pin_;
    bool enabled_ = false;
};
