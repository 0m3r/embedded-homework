#pragma once

#include <cstdint>

class DebugLogger {
public:
    void log(const char* message) const;
    void update() const;

private:
    static constexpr std::uint32_t QueueCapacity = 32;
    static constexpr std::uint32_t MessageSize = 96;

    mutable char queue_[QueueCapacity][MessageSize]{};
    mutable std::uint32_t head_ = 0;
    mutable std::uint32_t tail_ = 0;
    mutable std::uint32_t count_ = 0;
    mutable bool transmitting_ = false;
};
