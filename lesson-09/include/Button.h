#pragma once

#include <stdint.h>

class Button
{
public:
    constexpr Button(uint8_t pin, uint32_t debounceMs)
        : pin_(pin),
          debounceMs_(debounceMs)
    {
    }

    void init();
    void update(uint32_t now);
    bool consumePressEvent();

private:
    static void onChangeIsr();
    static volatile bool interruptEvent_;

    bool takeInterruptEvent();

    const uint8_t pin_;
    const uint32_t debounceMs_;

    uint32_t lastChangeAt_ = 0;
    bool rawState_ = true;
    bool stableState_ = true;
    bool pressedEvent_ = false;
};
