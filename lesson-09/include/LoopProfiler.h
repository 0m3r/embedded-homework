#pragma once

#include <stdint.h>

class LoopProfiler
{
public:
    explicit constexpr LoopProfiler(uint32_t reportIterations)
        : reportIterations_(reportIterations)
    {
    }

    void beginIteration(uint32_t nowUs);
    bool endIteration(uint32_t nowUs);

    uint32_t averageTimeUs() const;
    uint32_t iterationCount() const;

private:
    const uint32_t reportIterations_;

    uint32_t startedAtUs_ = 0;
    uint64_t totalTimeUs_ = 0;
    uint32_t currentCount_ = 0;

    uint32_t lastAverageTimeUs_ = 0;
    uint32_t lastIterationCount_ = 0;
};
