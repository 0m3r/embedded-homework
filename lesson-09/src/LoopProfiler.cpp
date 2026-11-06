#include "LoopProfiler.h"

void LoopProfiler::beginIteration(uint32_t nowUs)
{
    startedAtUs_ = nowUs;
}

bool LoopProfiler::endIteration(uint32_t nowUs)
{
    totalTimeUs_ += nowUs - startedAtUs_;
    ++currentCount_;

    if (currentCount_ < reportIterations_)
    {
        return false;
    }

    lastIterationCount_ = currentCount_;
    lastAverageTimeUs_ = static_cast<uint32_t>(
        totalTimeUs_ / currentCount_
    );

    totalTimeUs_ = 0;
    currentCount_ = 0;

    return true;
}

uint32_t LoopProfiler::averageTimeUs() const
{
    return lastAverageTimeUs_;
}

uint32_t LoopProfiler::iterationCount() const
{
    return lastIterationCount_;
}
