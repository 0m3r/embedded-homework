# Поліцейська мигалка

Проєкт керує червоним і синім світлодіодами та імітує поліцейську мигалку. Активні патерни вибираються бітовою маскою та повторюються безперервно.

## Підключення

| Компонент | GPIO | Підключення |
| --- | --- | --- |
| Червоний LED | 15 | Анод через резистор до GPIO 15, катод до GND |
| Синій LED | 40 | Анод через резистор до GPIO 40, катод до GND |

Кожен світлодіод повинен мати окремий струмообмежувальний резистор.

## Патерн

1. `MODE_ALTERNATING` — червоний і синій LED по черзі роблять чотири короткі спалахи.
2. `MODE_GROUPED` — червоний LED блимає тричі, потім синій LED блимає тричі.
3. `MODE_REVERSE` — два LED по черзі керуються через GPIO 4 в окремій FreeRTOS task.
4. `MODE_SOS` — обидва LED передають SOS (`··· --- ···`) через `morse::send()`.

Крапка SOS триває 180 мс, тире — 540 мс.

`MODE_REVERSE` працює паралельно з патернами на GPIO 15/40. Між reversed-спалахами GPIO 4 переходить у `INPUT`, тому обидва підключені до нього LED вимкнені.

Morse-абетка зараз містить символи `S` і `O`. Невідомі символи пропускаються.

```cpp
const std::initializer_list<uint8_t> MORSE_PINS = {
    led_controler::RED_PIN,
    led_controler::BLUE_PIN,
};

morse::send("SOS", MORSE_PINS);
```

## Структура коду

- `include/lesson4_task01.h` — публічний інтерфейс уроку.
- `src/lesson4_task01.cpp` — вибір режимів і реалізація уроку.
- `src/led_controler.h/.cpp` — GPIO, LED-патерни та FreeRTOS task.
- `src/morse.h/.cpp` — Morse-абетка й передавання повідомлень.
- `library.json` — опис локальної PlatformIO-бібліотеки.

## Вибір режимів

Режими можна комбінувати оператором `|`. `MODE_ALTERNATING`, `MODE_GROUPED` і `MODE_SOS` виконуються послідовно, а `MODE_REVERSE` працює паралельно в окремій FreeRTOS task.

```cpp
constexpr uint8_t ACTIVE_MODES =
    MODE_ALTERNATING | MODE_REVERSE | MODE_SOS;
```

Щоб залишити один режим, вкажіть `MODE_ALTERNATING`. Для двох режимів, наприклад, використайте `MODE_ALTERNATING | MODE_GROUPED`.

## Збірка та прошивка

Урок підключений як локальна бібліотека до головного PlatformIO-проєкту:

```text
/home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
```

Його `src/main.cpp` викликає `lesson4::task01::setup()` та `lesson4::task01::loop()`. Збірка й прошивка запускаються з головного проєкту:

```bash
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded --target upload
```
