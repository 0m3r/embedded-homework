# Керування LED кнопкою

Проєкт демонструє цифровий вхід з внутрішнім pull-up резистором. Натискання кнопки вмикає червоний LED, відпускання — вимикає. Стан кнопки виводиться в Serial Monitor зі швидкістю `115200` бод.

## Підключення

| Компонент | GPIO | Підключення |
| --- | --- | --- |
| Червоний LED | 15 | Анод через резистор 220 Ом до GPIO 15, катод до GND |
| Кнопка | 0 | Один контакт до GPIO 0, другий до GND |

Кнопка використовує `INPUT_PULLUP`, тому натиснутий стан відповідає `LOW`.

> GPIO 0 — strapping-пін. Не утримуйте кнопку натиснутою під час запуску або скидання плати: ESP32-S3 може перейти в режим прошивки.

## Структура коду

- `include/lesson4_task02.h` — публічний інтерфейс уроку.
- `src/lesson4_task02.cpp` — налаштування GPIO та керування LED.
- `library.json` — опис локальної PlatformIO-бібліотеки.

## Збірка та прошивка

Урок підключений як локальна бібліотека до головного PlatformIO-проєкту:

```text
/home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
```

Його `src/main.cpp` викликає `lesson4::task02::setup()` та `lesson4::task02::loop()`:

```bash
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded --target upload
```
