# Урок 12 — Модуль 2.4: Hardware Interrupts

## Класна робота

[`classwork/stm32`](classwork/stm32) — PlatformIO/C++ проєкт для Black Pill
STM32F411CE:

- кнопки `PA5` і `PB6` через EXTI;
- LED `PC13` (active-low) і `PC15` (active-high);
- неблокуючий debounce через `Clock` та `IntervalTimer`;
- події передаються з ISR у superloop;
- логи натискань через USB CDC Virtual COM Port.

ST-Link використовується для прошивання через SWD. USB-C плати передає логи.
