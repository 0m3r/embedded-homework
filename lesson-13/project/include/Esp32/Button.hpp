#pragma once

#include <cstdint>

class Button {
public:
    explicit Button(std::uint8_t pin);

    void begin();
    bool isPressed() const;
    std::uint32_t takeInterrupts();

private:
    static void interruptHandler(void* context);
    void onInterrupt();

    const std::uint8_t pin_;
    volatile std::uint32_t pendingInterrupts_ = 0;
};
