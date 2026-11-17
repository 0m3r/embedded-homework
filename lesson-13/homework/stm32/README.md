# STM32 timer homework

STM32F411 керує вентилятором через транзистор на `PC15`:

- цикл запуску — 1 година;
- час роботи — 15 хвилин;
- one-shot `TIM2` відміряє одразу весь інтервал;
- superloop використовується лише для USB CDC логування;
- `delay()` відсутній.

```text
OFF 3600 с → ON 900 с → OFF 2700 с → повтор
```

Щосекундного interrupt і програмного лічильника часу немає. `running_`
захищає від повторного запуску; LED на `PC15` дублює стан вентилятора.

USB CDC файли відокремлені в `include/Usb` та `src/Usb`.

Production:

```bash
pio run -e blackpill_f411ce
```

Debug — цикл 20 секунд, робота 5 секунд:

```bash
pio run -e blackpill_f411ce_debug
```
