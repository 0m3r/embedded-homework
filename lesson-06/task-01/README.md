# Код з уроку — ADC, LDR та фільтрація

ESP32 читає аналогове значення фоторезистора на GPIO2 кожні 10 мс. Moving average та median filter згладжують сигнал різними способами. LED на GPIO17 керується moving average з гістерезисом.

## Підключення

```text
3.3V ── LDR ──┬── GPIO2
              │
            10 kΩ
              │
             GND

GPIO17 ── 220 Ω ── LED ── GND
```

## Параметри

- ADC period: `10 мс`
- Debug output period: `500 мс`
- Moving average window: `4` вимірювання
- Median filter window: `5` вимірювань
- LED ON threshold: `2800`
- LED OFF threshold: `3200`
- Serial speed: `115200`

## Фільтри

`MovingAverage` зберігає суму останніх чотирьох значень. Нове значення додається, найстаріше віднімається, тому повний масив не потрібно щоразу сумувати.

`MedianFilter` зберігає п'ять значень, сортує їх копію та повертає середній елемент. Поодинокі великі стрибки майже не впливають на результат.

Serial monitor показує обидва результати:

```text
 RAW | AVERAGE | MEDIAN |  LED
-----+---------+--------+-----
3667 |    3665 |   3663 |  OFF
```

Заголовок таблиці повторюється кожні 25 рядків.

## Гістерезис

- нижче `2800` LED вмикається;
- вище `3200` LED вимикається;
- між `2800` і `3200` попередній стан зберігається.

Це запобігає мерехтінню LED, коли значення ADC коливається біля одного порога.

## Збірка та прошивка

```bash
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded
pio run -d /home/om3r/Documents/PlatformIO/Projects/esp32-s3-embedded --target upload
pio device monitor -p /dev/ttyACM0 -b 115200
```
