#pragma once

#include "Stm32/Button.hpp"
#include "Stm32/Config.hpp"
#include "Stm32/DebugLogger.hpp"
#include "Stm32/Led.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);

private:
    [[noreturn]] void fail(const char* message);

    IWDG_HandleTypeDef watchdog_{};
    DebugLogger logger_;
    Button button1_{
        GPIOB, GPIO_PIN_0, true, GPIO_PULLDOWN,
        TIM2, Config::DebounceMs};
    Button button2_{
        GPIOB, GPIO_PIN_6, true, GPIO_NOPULL};
    Led led1_{GPIOC, GPIO_PIN_13, false};
    Led led2_{GPIOC, GPIO_PIN_14, true};
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
