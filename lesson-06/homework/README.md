# Домашнє завдання — вимірювання освітленості через ADC

ESP32-S3 зчитує LDR на GPIO2 кожні 100 мс, обчислює напругу з RAW ADC, окремо вимірює її через `analogReadMilliVolts()` та показує відносну похибку.

## Підключення

```text
3.3V ── LDR ──┬── GPIO2
              │
            10 kΩ
              │
             GND
```

## Формули

```text
Ucalc = RAW × 3100 / 4095
Error = |Ucalc − Umeasured| / Umeasured × 100%
```

`ADCmax = 4095`, `Uref = 3100 мВ`. Якщо виміряна напруга дорівнює нулю, похибка виводиться як `N/A`.

## Вивід

```text
 RAW | CALC, mV | ADC, mV | ERROR, %
-----+----------+---------+---------
3667 |   2775.5 |    2790 |    0.52
```

Заголовок повторюється кожні 25 вимірювань.

## Збірка та прошивка

```bash
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded --target upload
pio device monitor -p /dev/ttyACM0 -b 115200
```
