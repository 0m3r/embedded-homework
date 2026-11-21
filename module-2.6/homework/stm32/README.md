# STM32 debounce comparison

Порівняння двох кнопок з класної роботи:

```text
PB0 without RC, internal pull-down -> PC13 active-low LED
PB6 with external RC pull-down     -> PC14 active-high LED
```

Кожна кнопка паралельно проходить чотири алгоритми:

1. `No debounce` — усі raw interrupts.
2. `Time-based` — не частіше однієї події за 50 мс.
3. `State FSM` — подія interrupt, 50 мс стабільності, повторна перевірка.
4. `Polling FSM` — опитування кожні 5 мс та 50 мс стабільності.

`State FSM` перемикає відповідний LED.

Збірка та прошивання:

```bash
pio run -e stm32f411
pio run -e stm32f411 -t upload
```

Логи через USB CDC:

```text
[PB0 no RC] No debounce: ...
[PB0 no RC] Time-based: ...
[PB0 no RC] State FSM: ...
[PB0 no RC] Polling FSM: ...
[PB6 hardware RC] No debounce: ...
```

Для вимірювання натиснути кожну кнопку однакову відому кількість разів і
порівняти лічильники.

## Результати

### PB0 без RC — 10 натискань

| Метод | Подій | Хибних |
| --- | ---: | ---: |
| No debounce | 19 | 9 |
| Time-based | 15 | 5 |
| State FSM | 10 | 0 |
| Polling FSM | 10 | 0 |

### PB6 з hardware RC — 10 натискань

| Метод | Подій | Хибних |
| --- | ---: | ---: |
| No debounce | 10 | 0 |
| Time-based | 10 | 0 |
| State FSM | 10 | 0 |
| Polling FSM | 10 | 0 |

## Порівняння

| Метод | Хибні спрацювання без RC | Затримка | Коментар |
| --- | ---: | --- | --- |
| No debounce | 9 | мінімальна | Один натиск створює кілька interrupts |
| Time-based | 5 | до 50 мс | Зменшує bounce, але не гарантує одну подію |
| State FSM | 0 | приблизно 50 мс | Interrupt запускає перевірку стабільного стану |
| Polling FSM | 0 | 50–55 мс | Стан кнопки перевіряється кожні 5 мс |
| Hardware RC | 0 | апаратна | PB6 дає одну raw-подію на натискання |

## Висновки

- PB0 без RC створив 9 зайвих raw interrupts.
- Time-based фільтр зменшив bounce, але залишив 5 хибних подій.
- State FSM і Polling FSM дали рівно одну подію на кожне натискання.
- Апаратний RC на PB6 повністю прибрав bounce у цьому тесті.
- ISR лише накопичує події; debounce, LED та USB-логування виконуються в
  superloop.

## Монітор із записом у файл

Для `stm32f411` увімкнений PlatformIO-фільтр `log2file`:

```bash
pio device monitor --port /dev/ttyACM1 --baud 115200
```

Логи одночасно показуються в терміналі та записуються в поточний каталог у
файл `logs/device-monitor-*.log`.
