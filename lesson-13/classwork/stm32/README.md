# STM32 timer classwork

Класна робота на Black Pill STM32F411CE:

- `PA5` перемикає зелений LED на `PC13`;
- `PB6` запускає або зупиняє миготіння червоного LED на `PC15`;
- `TIM2` створює період 500 мс;
- ISR лише накопичує завершені періоди;
- LED перемикається в superloop;
- після зупинки червоний LED вимикається.

```bash
pio run
pio run --target upload
```
