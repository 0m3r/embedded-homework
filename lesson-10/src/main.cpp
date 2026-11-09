#include <Arduino.h>

constexpr uint8_t LDR_PIN = 2;
constexpr uint8_t MOTOR_PIN = 17;

// Пороги освітленості: підлаштуй під свої вимірювання.
constexpr uint16_t THRESHOLD_DARK = 1800;
constexpr uint16_t THRESHOLD_LIGHT = 2200;

// Кількість вимірювань для ковзного середнього.
constexpr uint8_t FILTER_SIZE = 8;

// Інтервали часу.
constexpr uint32_t ADC_INTERVAL_MS = 20;
constexpr uint32_t PRINT_INTERVAL_MS = 500;
constexpr uint32_t CONFIRM_TIME_MS = 500;

static_assert(
    FILTER_SIZE > 0,
    "FILTER_SIZE must be greater than zero"
);

static_assert(
    THRESHOLD_DARK < THRESHOLD_LIGHT,
    "THRESHOLD_DARK must be less than THRESHOLD_LIGHT"
);

// Дані для усереднення.
uint16_t readings[FILTER_SIZE] = {};
uint32_t sum = 0;
uint8_t readingIndex = 0;
uint8_t readingCount = 0;

// Останні покази датчика.
uint16_t rawValue = 0;
uint16_t filteredValue = 0;

// Стан мотора та очікування перемикання.
bool motorOn = false;
bool switchPending = false;

// Мітки часу.
uint32_t lastAdcTime = 0;
uint32_t lastPrintTime = 0;
uint32_t pendingSince = 0;

// Обчислення середнього останніх FILTER_SIZE вимірювань.
uint16_t filterADC(uint16_t value)
{
    // Віднімаємо найстаріше значення.
    sum -= readings[readingIndex];

    // Записуємо нове значення та додаємо його до суми.
    readings[readingIndex] = value;
    sum += value;

    // Перехід до наступної комірки по колу: 0...7, потім 0.
    readingIndex = (readingIndex + 1) % FILTER_SIZE;

    // Під час заповнення враховуємо лише отримані вимірювання.
    if (readingCount < FILTER_SIZE)
    {
        ++readingCount;
    }

    return static_cast<uint16_t>(sum / readingCount);
}

// Гістерезис із підтвердженням зміни освітлення.
void updateMotor(uint16_t value, uint32_t now)
{
    bool requestedState = motorOn;

    if (value < THRESHOLD_DARK)
    {
        requestedState = true;
    }
    else if (value > THRESHOLD_LIGHT)
    {
        requestedState = false;
    }

    // Якщо зміна не потрібна або покази між порогами,
    // скасовуємо очікування та зберігаємо стан мотора.
    if (requestedState == motorOn)
    {
        switchPending = false;
        return;
    }

    // Умова перемикання виникла: починаємо відлік.
    if (!switchPending)
    {
        switchPending = true;
        pendingSince = now;
        return;
    }

    // Перемикаємо мотор, якщо умова тримається 500 мс.
    if (now - pendingSince >= CONFIRM_TIME_MS)
    {
        motorOn = requestedState;

        // HIGH через резистор відкриває 2N2222 і вмикає мотор.
        // 1N4007 паралельно мотору гасить імпульс самоіндукції.
        digitalWrite(MOTOR_PIN, motorOn ? HIGH : LOW);

        switchPending = false;
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LDR_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);

    // Початковий стан: мотор вимкнений.
    digitalWrite(MOTOR_PIN, LOW);

    // 12-бітний ADC: значення від 0 до 4095.
    analogReadResolution(12);
    analogSetPinAttenuation(LDR_PIN, ADC_11db);
}

void loop()
{
    const uint32_t now = millis();

    // Зчитування датчика та керування мотором кожні 20 мс.
    if (now - lastAdcTime >= ADC_INTERVAL_MS)
    {
        lastAdcTime = now;

        rawValue = analogRead(LDR_PIN);
        filteredValue = filterADC(rawValue);

        updateMotor(filteredValue, now);
    }

    // Виведення інформації кожні 500 мс.
    if (now - lastPrintTime >= PRINT_INTERVAL_MS)
    {
        lastPrintTime = now;

        Serial.print("ADC raw: ");
        Serial.print(rawValue);

        Serial.print(" | ADC avg: ");
        Serial.print(filteredValue);

        Serial.print(" | Motor: ");
        Serial.print(motorOn ? "ON" : "OFF");

        Serial.print(" | Pending: ");
        Serial.println(switchPending ? "YES" : "NO");
    }
}
