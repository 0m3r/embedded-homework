# ESP32-S3 debounce

```text
GPIO21 button (INPUT_PULLUP) -> GPIO15 LED
```

GPIO interrupt перезапускає one-shot hardware timer на `50 мс`. Після
завершення відліку стан кнопки перевіряється повторно. Стабільне натискання
перемикає LED.

```bash
pio run
pio run -t upload
```
