#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
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
    potentiometer_.init();
    motorPwm_.init(micros());
    uartExperiment_.init();
    logger_.log("Optional motor software PWM homework");
}

void Application::loop()
{
    const uint32_t nowUs = micros();
    const uint32_t nowMs = millis();

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
