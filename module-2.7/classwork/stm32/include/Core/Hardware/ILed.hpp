#pragma once

class ILed {
public:
    virtual ~ILed() = default;

    virtual void off() = 0;
    virtual void on() = 0;
    virtual void toggle() = 0;
};
