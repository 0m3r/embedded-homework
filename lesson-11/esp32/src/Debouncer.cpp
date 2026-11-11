#include "Debouncer.h"

void Debouncer::init(bool value)
{
    input_ = value;
    state_ = value;
    timer_.reset();
}

bool Debouncer::update(bool value)
{
    if (value != input_)
    {
        input_ = value;
        timer_.reset();
        return false;
    }

    if (value == state_ || !timer_.ready())
    {
        return false;
    }

    state_ = value;
    return true;
}

bool Debouncer::state() const
{
    return state_;
}
