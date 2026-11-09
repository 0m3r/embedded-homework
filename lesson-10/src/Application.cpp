#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      relayTester_(
          Config::RelayControlPin,
          Config::RelayContactPin,
          Config::RelayMeasurementCount,
          Config::RelayDebounceUs,
          Config::RelaySettleUs,
          Config::RelayTimeoutUs
      ),
      potentiometer_(Config::PotentiometerPin, Config::AdcIntervalMs),
      motorPwm_(Config::MotorPin, Config::SoftwarePwmPeriodUs),
      uartExperiment_(
          Serial1,
          Config::UartTxPin,
          Config::UartBaud,
          Config::UartPatternIntervalMs
      )
{
}

void Application::setup()
{
    logger_.begin();

    const uint32_t nowUs = micros();
    relayTester_.init(nowUs);
    potentiometer_.init();
    motorPwm_.init(nowUs);
    uartExperiment_.init();

    logger_.log("Module 2.2: relay timing + software PWM");
    logger_.log("Starting 10 ON and 10 OFF relay measurements...");
}

void Application::loop()
{
    const uint32_t nowUs = micros();
    const uint32_t nowMs = millis();

    relayTester_.update(nowUs);

    RelayMeasurement measurement{};
    if (relayTester_.takeMeasurement(measurement))
    {
        reportRelayMeasurement(measurement);
    }

    if (relayTester_.complete() && !relaySummaryPrinted_)
    {
        relaySummaryPrinted_ = true;
        logger_.logf(
            "Relay averages: ON = %lu us (%.3f ms), OFF = %lu us (%.3f ms)\n",
            static_cast<unsigned long>(relayTester_.averageOnUs()),
            relayTester_.averageOnUs() / 1000.0,
            static_cast<unsigned long>(relayTester_.averageOffUs()),
            relayTester_.averageOffUs() / 1000.0
        );
    }

    if (relayTester_.failed() && !relayErrorPrinted_)
    {
        relayErrorPrinted_ = true;
        logger_.log("Relay timeout: check COM/NO wiring and shared GND");
    }

    if (potentiometer_.update(nowMs))
    {
        motorPwm_.setDuty(potentiometer_.duty());
    }

    motorPwm_.update(nowUs);
    uartExperiment_.update(nowMs);

    if (nowMs - lastPrintAtMs_ >= Config::PrintIntervalMs)
    {
        lastPrintAtMs_ = nowMs;
        logger_.logf(
            "ADC: %u | PWM: %u/255 (%u%%) | UART: 0x%02X\n",
            static_cast<unsigned>(potentiometer_.value()),
            static_cast<unsigned>(motorPwm_.duty()),
            static_cast<unsigned>(motorPwm_.duty()) * 100U / 255U,
            static_cast<unsigned>(uartExperiment_.currentPattern())
        );
    }
}

void Application::reportRelayMeasurement(
    const RelayMeasurement& measurement
)
{
    logger_.logf(
        "Relay %s #%u: %lu us (%.3f ms)\n",
        measurement.transition == RelayTransition::TurnOn ? "ON" : "OFF",
        static_cast<unsigned>(measurement.number),
        static_cast<unsigned long>(measurement.durationUs),
        measurement.durationUs / 1000.0
    );
}
