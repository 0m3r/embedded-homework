#include "Application.hpp"
#include "usb_device.h"

extern "C" PCD_HandleTypeDef hpcd_USB_OTG_FS;

namespace {
void systemClockInit()
{
    RCC_OscInitTypeDef oscillator = {};
    RCC_ClkInitTypeDef clocks = {};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    oscillator.PLL.PLLState = RCC_PLL_ON;
    oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscillator.PLL.PLLM = 25;
    oscillator.PLL.PLLN = 192;
    oscillator.PLL.PLLP = RCC_PLLP_DIV2;
    oscillator.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
        while (true) {}
    }

    clocks.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                       RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clocks.APB1CLKDivider = RCC_HCLK_DIV2;
    clocks.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_3) != HAL_OK) {
        while (true) {}
    }
}

void gpioInit()
{
    GPIO_InitTypeDef gpio = {};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

    gpio.Pin = GPIO_PIN_13 | GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio);

    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_IT_RISING;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOB, &gpio);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
}

int main()
{
    HAL_Init();
    systemClockInit();
    gpioInit();
    MX_USB_DEVICE_Init();
    appInit();

    while (true) {
        appUpdate();
    }
}

extern "C" void SysTick_Handler()
{
    HAL_IncTick();
}

extern "C" void EXTI9_5_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

extern "C" void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

extern "C" void Error_Handler()
{
    __disable_irq();
    while (true) {}
}

extern "C" void HAL_GPIO_EXTI_Callback(std::uint16_t pin)
{
    appButtonInterrupt(pin);
}
