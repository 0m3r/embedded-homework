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

## Домашня робота

[`homework/esp32`](homework/esp32) — порівняння debounce для Freenove
ESP32-S3: без debounce, time-based, state-based, polling та hardware RC.
README містить результати вимірювань, порівняльну таблицю й висновки.

[`homework/stm32`](homework/stm32) — те саме дослідження для Black Pill
STM32F411CE з software-кнопкою `PA5`, hardware RC на `PB6`, чотирма
debounce-стратегіями та USB CDC логуванням.
