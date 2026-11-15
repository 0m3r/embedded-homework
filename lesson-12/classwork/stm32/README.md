# STM32F411 buttons and LEDs

C++ версія навчального прикладу для Black Pill `STM32F411CE`.

- `PC13` — червоний LED, active-low;
- `PC15` — зелений LED, active-high;
- `PA5` — червона кнопка;
- `PB6` — зелена кнопка;
- debounce — `100 мс`.

`Clock` централізує `HAL_GetTick()`. Кожна кнопка має власний
`IntervalTimer` для неблокуючого debounce.

USB CDC передає повідомлення через USB-C плати у Virtual COM Port:

```text
Red button pressed
Green button pressed
```

У Linux STM32 з'являється як `/dev/ttyACM*` (`0483:5740`). ST-Link
використовується для прошивання через SWD.


## VS Code / PlatformIO

- ✓ — Build;
- → — Upload через активний `upload_protocol` у `platformio.ini`;
- 🐞 — Debug через ST-Link.

CLI:

```bash
pio run
pio run --target upload
pio device monitor --port /dev/ttyACM1 --baud 115200
```

Для DFU переведи плату в системний bootloader через `BOOT0`, підключи USB-C,
а потім запускай Upload. Після прошивки поверни `BOOT0` у звичайний режим і
перезапусти плату.
