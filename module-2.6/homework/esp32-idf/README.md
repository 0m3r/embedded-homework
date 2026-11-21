# ESP32-S3 debounce на ESP-IDF

```text
GPIO21 button (active-low, internal pull-up) -> GPIO15 LED
```

Одна фізична кнопка паралельно перевіряється чотирма простими методами:

| Лог | Метод |
| --- | --- |
| `No debounce` | Кожен raw interrupt |
| `Time-based` | 50 мс між прийнятими натисканнями |
| `Timer FSM` | Interrupt + one-shot timer + явна FSM |
| `Polling FSM` | Опитування кожні 5 мс + явна FSM |

`Timer FSM` перемикає LED. Його стани:

```text
Released -> DebouncingPress -> Pressed
Pressed  -> DebouncingRelease -> Released
```

GPIO ISR лише додає рівень кнопки до FreeRTOS queue. Логування, FSM та
керування LED виконуються поза ISR. `delay()` не використовується.

Класи розділені за відповідальністю:

- `Button` — GPIO, ISR, queue та читання входу;
- `NoDebouncer` і `TimeBasedDebouncer` — прості методи порівняння;
- `TimerFsmDebouncer` — подієва FSM з `esp_timer`;
- `PollingFsmDebouncer` — FSM на періодичному опитуванні.

## Arduino та ESP-IDF

Arduino classwork використовує `setup()`, `loop()`, `pinMode()` і
`attachInterrupt()`. Цей homework використовує нативні ESP-IDF API:
`app_main()`, GPIO driver, FreeRTOS queue та `esp_timer`.

## Збірка

```bash
pio run
```

Прошивання та монітор:

```bash
pio run -t upload
pio device monitor
```
