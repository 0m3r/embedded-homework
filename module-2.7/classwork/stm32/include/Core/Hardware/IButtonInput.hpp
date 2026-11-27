#pragma once

class IButtonInput {
public:
    virtual ~IButtonInput() = default;

    virtual bool isPressed() const = 0;
};
