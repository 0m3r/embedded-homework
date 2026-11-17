# ESP32 timer classwork

- `GPIO21` запускає або зупиняє миготіння LED на `GPIO13`;
- hardware timer створює період 500 мс;
- ISR лише накопичує завершені періоди;
- LED перемикається в `loop()`;
- `delay()` відсутній.

```bash
pio run
pio run -t upload
```
