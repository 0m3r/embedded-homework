# Модуль 2.7 — світлофор на ESP32-S3

Мініпроєкт реалізує цикл сигналів відповідно до описаної в завданні
послідовності ПДР України:

```text
Green
  -> Green flashing
  -> Yellow
  -> Red
  -> Red + Yellow
  -> Green
```

Кнопка перемикає звичайний режим і особливий режим миготливого жовтого.
Після виходу з особливого режиму цикл безпечно починається з червоного.

## Підключення

Усі LED active-high, кожен підключається через струмообмежувальний резистор.
Кнопка active-high використовує внутрішній pull-down.

```text
GPIO12 -> resistor -> red LED    -> GND
GPIO16 -> resistor -> yellow LED -> GND
GPIO17 -> resistor -> green LED  -> GND
GPIO21 -> button -> 3.3 V
```

## Стани

Основна FSM:

- `Green`;
- `GreenFlashingOn` / `GreenFlashingOff`;
- `Yellow`;
- `Red`;
- `RedYellow`;
- `FlashingYellowOn` / `FlashingYellowOff`.

FSM debounce кнопки:

```text
Idle -> Pressed -> Held -> Release -> Idle
```

## Таймери

- `delay()` не використовується;
- `Application` запускає одну FreeRTOS task, як у попередньому проєкті;
- один one-shot `esp_timer` перевикористовується для всіх станів світлофора;
- спільна робота з таймером винесена в клас `OneShotTimer`;
- кожен стан запускає таймер зі своєю тривалістю;
- callback таймера лише виставляє ознаку завершення;
- наступний стан і GPIO перемикаються в `Application` task;
- ISR кнопки лише записує raw edge у queue;
- `TimerFsmDebouncer` запускає one-shot `esp_timer` на 50 мс;
- callback таймера виставляє запит на зміну режиму;
- вся логіка станів, GPIO та логування виконується поза ISR.

Демонстраційні інтервали налаштовуються в `include/EspIdf/Config.hpp`.

| Сигнал | Тривалість |
|---|---:|
| Зелений | 5 с |
| Зелений миготливий | 3 с, півперіод 500 мс |
| Жовтий | 2 с |
| Червоний | 5 с |
| Червоний + жовтий | 1 с |
| Особливий жовтий | півперіод 500 мс |

## Збірка та запуск

```bash
pio run
pio run -t upload
pio device monitor
```

Монітор показує назву кожного активного стану. `log2file` одночасно записує
результат у файл для подальшого додавання до звіту.
