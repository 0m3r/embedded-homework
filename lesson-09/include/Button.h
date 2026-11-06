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
    bool update(uint32_t now);

private:
    static void onChangeIsr();
    static volatile bool interruptPending_;

    bool consumeInterruptEvent();
    bool debounce(uint32_t now);

    const uint8_t pin_;
    const uint32_t debounceMs_;

    uint32_t lastChangeAt_ = 0;
    bool rawPressed_ = false;
    bool stablePressed_ = false;
};
