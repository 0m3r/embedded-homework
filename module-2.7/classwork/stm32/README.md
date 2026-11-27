# Модуль 2.7 — Event-driven світлофор на STM32F411

Класна робота демонструє event-driven архітектуру на WeAct BlackPill
STM32F411CE:

```text
HAL ISR callbacks
    -> Stm32EventProducer
    -> EventQueue (FIFO)
    -> Stm32EventDispatcher
    -> IEventHandler
    -> TrafficLight event consumer / FSM
```

ISR лише перетворюють апаратні сигнали на події та додають їх у FIFO.
Debounce, переходи FSM і керування GPIO виконуються в main context.
Коли черга порожня, `Stm32EventDispatcher` очікує interrupt через `__WFI()`.

## Підключення

```text
PC13 -> resistor -> red LED    -> GND (active-high)
PC14 -> resistor -> yellow LED -> GND (active-high)
PC15 -> resistor -> green LED  -> GND (active-high)
PB0  -> button -> 3.3 V              (active-high, internal pull-down)
```

## Події

- `ButtonEdge` — edge кнопки `PB0`;
- `DebounceTimeout` — завершення one-shot TIM3;
- `TrafficTimeout` — завершення one-shot TIM2.

`Stm32EventProducer` знає про STM32 HAL, GPIO, таймери та додає timestamp.
`IEventDispatcher` задає platform-independent API. `Stm32EventDispatcher`
реалізує FIFO synchronization через PRIMASK, доставку подій та race-safe
очікування через `__WFI()`. `TrafficLight` реалізує `IEventHandler` напряму.
Реєстрація виконується явно в `Application`.

## Core framework

Platform-independent код винесено з `Stm32` у спільні `include/Core` та
`src/Core`:

- `Event`, `EventQueue`, `IEventDispatcher`, `IEventHandler`;
- `ILed`, `IButtonInput`, `IOneShotTimer`, `IClock`;
- `DebouncedButton`, `LedFlasher`, `IntervalTimer`.

`Core` не містить STM32 HAL, GPIO pins або конкретних таймерів. `Led`,
`ButtonInput`, `OneShotTimer`, `Stm32EventProducer` і
`Stm32EventDispatcher` є STM32 adapters. Ідентифікатори подій світлофора
залишаються в `Application/TrafficEvents.hpp`, а не в generic event layer.

## FSM

```text
Green
    -> GreenFlashing
    -> Yellow
    -> Red
    -> RedYellow
    -> Green
```

Кнопка перемикає звичайний режим і безперервний `FlashingYellow`.
Після виходу зі спеціального режиму цикл починається з червоного.

`LedFlasher` окремо керує фазою LED, кількістю півперіодів і повторним
запуском one-shot таймера:

- зелений — 6 півперіодів по 500 мс, тобто 3 миготіння;
- спеціальний жовтий — безперервно, півперіод 500 мс.

FSM debounce кнопки:

```text
Idle -> Pressed -> Held -> Release -> Idle
```

## Часові інтервали

| Сигнал | Тривалість |
|---|---:|
| Зелений | 5 с |
| Зелений миготливий | 3 с |
| Жовтий | 2 с |
| Червоний | 5 с |
| Червоний + жовтий | 1 с |
| Спеціальний жовтий | півперіод 500 мс |
| Debounce кнопки | 50 мс |

## Збірка

```bash
pio run -e stm32f411
pio run -e stm32f411 -t upload
pio device monitor
```

Прошивання використовує ST-Link. Логи передаються через USB CDC.
