# Модуль 2.7 — світлофор на STM32F411

Мініпроєкт для WeAct BlackPill STM32F411CE реалізує цикл:

```text
Green
  -> Green flashing
  -> Yellow
  -> Red
  -> Red + Yellow
  -> Green
```

Кнопка перемикає звичайний режим і особливий режим миготливого жовтого.
Після виходу з особливого режиму цикл безпечно починається з червоного.

## Підключення

Кожен зовнішній LED підключається через струмообмежувальний резистор.
Кнопка active-high використовує внутрішній pull-down.

```text
PC13 -> cathode of red LED; anode -> resistor -> 3.3 V (active-low)
PC14 -> resistor -> yellow LED -> GND                 (active-high)
PC15 -> resistor -> green LED  -> GND                 (active-high)
PB0  -> button -> 3.3 V                               (active-high)
```

## FSM

Стани світлофора:

- `Green`;
- `GreenFlashingOn` / `GreenFlashingOff`;
- `Yellow`;
- `Red`;
- `RedYellow`;
- `FlashingYellowOn` / `FlashingYellowOff`.

FSM debounce кнопки:

```text
Idle -> Pressed -> Held -> Release -> Idle
```

## Таймери та переривання

- `delay()` і блокувальне очікування не використовуються;
- TIM2 працює як one-shot таймер станів світлофора;
- TIM3 працює як one-shot таймер debounce на 50 мс;
- обидва таймери мають частоту лічильника 10 кГц, тобто 10 ticks/ms;
- `OneShotTimer` зупиняє таймер після update interrupt;
- EXTI кнопки лише накопичує raw edge;
- ISR TIM2/TIM3 лише фіксують завершення таймера;
- debounce, переходи FSM, LED і логування виконуються в superloop.

| Сигнал | Тривалість |
|---|---:|
| Зелений | 5 с |
| Зелений миготливий | 3 с, півперіод 500 мс |
| Жовтий | 2 с |
| Червоний | 5 с |
| Червоний + жовтий | 1 с |
| Особливий жовтий | півперіод 500 мс |

## Збірка та запуск

```bash
pio run -e stm32f411
pio run -e stm32f411 -t upload
pio device monitor
```

Прошивання використовує ST-Link. Логи передаються через USB CDC; фільтр
`log2file` одночасно записує їх у `logs/device-monitor-*.log`.
