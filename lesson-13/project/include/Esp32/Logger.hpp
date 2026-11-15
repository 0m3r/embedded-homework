#pragma once

#include <cstdint>

#include "Debouncer/IReporter.hpp"

class Logger : public IReporter {
public:
    void begin(std::uint32_t baud) const;
    void started(std::uint8_t pin) const;
    void press(const char* method, std::uint32_t count) const override;
};
