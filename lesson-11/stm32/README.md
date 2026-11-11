# STM32F411 — Superloop

STM32CubeMX/CMake-проєкт для плати на `STM32F411CEU6`.

- LED 1: вбудований `PC13`, active-low, інтервал `250` мс;
- LED 2: зовнішній `PB0`, active-high, інтервал `500` мс;
- LED 3: зовнішній `PB1`, active-high, інтервал `1000` мс;
- джерело часу: `HAL_GetTick()`;
- головний цикл: `Application::update()`;
- блокувальний `HAL_Delay()` не використовується.

CubeMX залишає точку входу в `main.c`. C++-логіка викликається через функції
з C-сумісним інтерфейсом:

```c
app_init();

while (1) {
    app_update();
}
```

Збірка:

```bash
cube-cmake --preset Debug
cube-cmake --build --preset Debug
```

Зовнішні LED підключаються так:

```text
PB0/PB1 → резистор 220–330 Ом → анод LED
катод LED → GND
```
