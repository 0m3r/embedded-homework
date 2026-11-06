# Урок 09 — Модуль 2.1

## Домашнє завдання: Embedded C++ на ESP32

Неблокуюче керування світлодіодом у superloop-архітектурі. Кнопка через
переривання перемикає режими роботи LED:

```text
Blink → AlwaysOn → AlwaysOff → Blink
```

### Реалізовано

- `enum class LedState` та `enum class LedMode`;
- параметри застосунку як `static constexpr` у `Config`;
- клас `Led` з методами `init()` і `set()`;
- клас `Button` з мінімальним ISR, `volatile`-прапорцем і debounce;
- клас `LedController` для режимів і неблокуючого блимання через `millis()`;
- клас `LoopProfiler` для вимірювання середнього часу superloop;
- локальні `static`-об'єкти замість глобальних об'єктів;
- відсутність `delay()`, dynamic allocation і STL-контейнерів;
- `static_assert` для перевірки конфігурації під час компіляції.

### Структура

| Компонент | Відповідальність |
| --- | --- |
| `Config` | GPIO, часові інтервали та Serial baud rate |
| `Led` | Фізичний стан світлодіода |
| `Button` | Переривання, debounce і подія натискання |
| `LedController` | Режими LED та таймер блимання |
| `LoopProfiler` | Середній час виконання ітерації superloop |

### Підключення

```text
GPIO 16 ── резистор 220–330 Ω ── анод LED
GND     ──────────────────────── катод LED

GPIO 17 ── кнопка ── GND
```

Кнопка використовує `INPUT_PULLUP`, тому зовнішній підтягувальний резистор
не потрібен. Відпущена кнопка має рівень `HIGH`, натиснута — `LOW`.

### Збірка

```bash
pio run
```

Плата: Freenove ESP32-S3 WROOM, Arduino framework.
