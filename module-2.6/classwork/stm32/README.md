# STM32F411 debounce

```text
PB0 button (active-high)
  -> TIM2 software debounce 50 ms
  -> PC13 active-low LED

PB6 button (active-high, external RC debounce)
  -> PC14 active-high LED
```

PB0 interrupt запускає TIM2. Після `50 мс` GPIO перевіряється повторно.
PB6 використовує зовнішній RC-фільтр, тому software debounce для нього не
застосовується.

```bash
pio run
pio run -t upload
```
