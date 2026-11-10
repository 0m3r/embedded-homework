#pragma once

#include <stdint.h>

class RelayContact
{
public:
    constexpr RelayContact(uint8_t pin, uint32_t debounceUs)
        : pin_(pin),
          debounceUs_(debounceUs)
    {
    }

    void init();
    bool tick(uint32_t nowUs);
    bool isClosed() const;
    uint32_t changedAtUs() const;

private:
    static void onChangeIsr();
    static volatile bool changed_;
    static volatile uint32_t edgeAtUs_;

    bool consumeInterrupt(uint32_t& edgeAtUs);
    bool debounce(uint32_t nowUs);

    const uint8_t pin_;
    const uint32_t debounceUs_;

    bool closed_ = false;
    bool pending_ = false;
    bool pendingClosed_ = false;
    uint32_t pendingAtUs_ = 0;
    uint32_t changedAtUs_ = 0;
};
