# Модуль 2.7 — Event-driven світлофор на STM32F411

Класна робота демонструє event-driven архітектуру на WeAct BlackPill
STM32F411CE:

```text
HAL ISR callbacks
    -> EventProducer
    -> EventQueue (FIFO)
    -> EventDispatcher
    -> IEventHandler
    -> TrafficLight event consumer / FSM
```

ISR лише перетворюють апаратні сигнали на події та додають їх у FIFO.
Debounce, переходи FSM і керування GPIO виконуються в main context.
Коли черга порожня, `Stm32EventWaiter` очікує interrupt через `__WFI()`.

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

`EventProducer` знає про STM32 HAL, GPIO, таймери та додає timestamp.
`EventDispatcher` і `EventQueue` залишаються незалежними від hardware та
доставляють події зареєстрованим реалізаціям `IEventHandler`. `TrafficLight`
реалізує цей інтерфейс напряму. Реєстрація виконується явно в `Application`.

Platform-specific операції винесені в адаптери:

- `Stm32CriticalSection` реалізує `ICriticalSection` через PRIMASK;
- `Stm32EventWaiter` виконує race-safe перевірку FIFO та `__WFI()`;
- `EventProducer` перетворює HAL callbacks на події.

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
