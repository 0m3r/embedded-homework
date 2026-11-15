# MultiBoard PlatformIO project

Один VS Code / PlatformIO-проєкт для двох контролерів:

| Environment | Board | Framework |
| --- | --- | --- |
| `esp32s3` | Freenove ESP32-S3 | Arduino |
| `stm32f411` | Black Pill STM32F411CE | STM32Cube |

```text
include/               headers
src/
├── Debouncer/         shared C++ logic
├── Esp32/             Arduino adapters and entry point
└── Stm32/             HAL, USB CDC and entry point
```

Build both targets:

```bash
pio run
```

Build or upload one target:

```bash
pio run -e esp32s3
pio run -e esp32s3 -t upload

pio run -e stm32f411
pio run -e stm32f411 -t upload
```

PlatformIO toolbar environment selector chooses active controller in VS Code.
Shared code must not include Arduino or STM32 HAL headers; platform-specific
GPIO, interrupts and logging stay under `Esp32` and `Stm32`.
