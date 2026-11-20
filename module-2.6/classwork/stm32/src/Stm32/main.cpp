#include "Stm32/Application.hpp"
#include "Stm32/SystemClock.hpp"
#include "usb_device.h"

extern "C" PCD_HandleTypeDef hpcd_USB_OTG_FS;

int main()
{
    HAL_Init();
    if (!SystemClock::init()) {
        Error_Handler();
    }
    MX_USB_DEVICE_Init();
    appInit();

    while (true) {
        appUpdate();
    }
}

extern "C" void EXTI0_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

extern "C" void EXTI9_5_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

extern "C" void HAL_GPIO_EXTI_Callback(std::uint16_t pin)
{
    appButtonInterrupt(pin);
}

extern "C" void SysTick_Handler()
{
    HAL_IncTick();
}

extern "C" void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

extern "C" void Error_Handler()
{
    NVIC_SystemReset();
}
