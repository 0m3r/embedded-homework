#pragma once

#include <cstdint>

class IReporter {
public:
    virtual ~IReporter() = default;
    virtual void press(const char* method, std::uint32_t count) const = 0;
};
