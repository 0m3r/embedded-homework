# STM32 timer homework

STM32F411 керує вентилятором через транзистор на `PC15`:

- цикл запуску — 1 година;
- час роботи — 15 хвилин;
- `TIM2` виконує планування незалежно від superloop;
- superloop використовується лише для USB CDC логування;
- `delay()` відсутній.

USB CDC файли відокремлені в `include/Usb` та `src/Usb`.

Production:

```bash
pio run -e blackpill_f411ce
```

Debug — цикл 20 секунд, робота 5 секунд:

```bash
pio run -e blackpill_f411ce_debug
```
