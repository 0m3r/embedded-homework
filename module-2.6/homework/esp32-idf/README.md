# ESP32-S3 debounce на ESP-IDF

```text
GPIO21 button (active-low, internal pull-up) -> GPIO15 LED
```

Алгоритм без `delay()`:

```text
GPIO ISR
-> FreeRTOS queue
-> debounce task restarts one-shot esp_timer
-> after 50 ms read GPIO again
-> stable press toggles LED
```

ISR лише додає подію до черги. Робота з таймером і LED виконується поза ISR.

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
