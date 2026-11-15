# STM32F411 buttons and LEDs

C++ версія навчального прикладу для Black Pill `STM32F411CE`.

- `PC13` — зелений LED, active-high;
- `PC15` — червоний LED, active-high;
- `PA5` — кнопка зеленого LED;
- `PB6` — кнопка червоного LED;
- debounce — `100 мс`, polling — кожні `5 мс`.

`PA5` використовує внутрішній pull-down та interrupt `RISING`.
`PB6` використовує зовнішній RC-фільтр та interrupt `FALLING`:

```text
3.3V ── 10 kOhm ──┬── PB6
                   ├── 100 nF (104) ── GND
                   └── button ── 0.99 kOhm ── GND
```

## Архітектура debounce

Обидві кнопки паралельно проходять однакові чотири алгоритми:

- `NoDebounceCounter` — рахує кожний raw interrupt;
- `TimeBasedDebouncer` — приймає події не частіше ніж раз на `100 мс`;
- `StateBasedDebouncer` — перевіряє стабільний стан через `100 мс`;
- `PollingDebouncer` — машина станів із polling кожні `5 мс`.

Класи лежать у `include/Debouncer` і `src/Debouncer` та реалізують спільний
C++-інтерфейс `IDebouncer` з обов'язковим методом `debounce()`.
`Button` лише читає GPIO та безпечно передає raw-події з ISR у superloop.
`ButtonExperiment` об'єднує одну кнопку, LED і чотири алгоритми, тому
`Application` містить лише software та hardware RC експерименти. LED
перемикається після підтвердженої події `State-based`.

USB CDC передає повідомлення через USB-C плати у Virtual COM Port:

```text
[PA5 software] No debounce: 4
[PA5 software] Time-based: 1
[PA5 software] State-based: 1
[PA5 software] Polling: 1
[PB6 hardware RC] No debounce: 1
[PB6 hardware RC] Time-based: 1
[PB6 hardware RC] State-based: 1
[PB6 hardware RC] Polling: 1
```

`DebugLogger` використовує чергу на 32 повідомлення, бо USB CDC не може
передати всі повідомлення одночасно без втрат. Raw interrupts одного проходу
superloop агрегуються в один рядок.

## Результати вимірювань

### PA5 без RC — 10 натискань

| Метод | Подій | Хибних |
| --- | ---: | ---: |
| No debounce | 13 | 3 |
| Time-based | 11 | 1 |
| State-based | 10 | 0 |
| Polling | 10 | 0 |

### PB6 з hardware RC — 12 натискань

| Метод | Подій | Хибних |
| --- | ---: | ---: |
| No debounce | 12 | 0 |
| Time-based | 12 | 0 |
| State-based | 12 | 0 |
| Polling | 12 | 0 |

## Висновки

- Без debounce механічний bounce створив 3 зайві interrupts на `PA5`.
- Time-based фільтр зменшив кількість помилок, але залишив одну зайву подію.
- State-based і polling дали рівно одну подію на кожне натискання.
- Hardware RC `10 кОм + 100 нФ` на `PB6` прибрав bounce у цьому тесті:
  навіть raw interrupt count збігся з реальною кількістю натискань.
- ISR лише накопичує raw-події; debounce, LED та USB-логування виконуються в
  superloop.

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
$HOME/.platformio/penv/bin/platformio device monitor --port /dev/ttyACM0 --baud 115200
```

Для DFU переведи плату в системний bootloader через `BOOT0`, підключи USB-C,
а потім запускай Upload. Після прошивки поверни `BOOT0` у звичайний режим і
перезапусти плату.
