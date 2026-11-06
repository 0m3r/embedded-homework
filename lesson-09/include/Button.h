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
    static volatile bool changed_;

    bool consumeChange();
    bool debounce(uint32_t now);

    const uint8_t pin_;
    const uint32_t debounceMs_;

    uint32_t lastChangeAt_ = 0;
    bool raw_ = false;
    bool stable_ = false;
};
