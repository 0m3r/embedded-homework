#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      potentiometer_(clock_, Config::PotentiometerPin, Config::AdcIntervalMs),
      motorPwm_(clock_, Config::MotorPin, Config::SoftwarePwmPeriodUs),
      uartExperiment_(
          clock_,
          Serial1,
          Config::UartTxPin,
          Config::UartBaud,
          Config::UartPatternIntervalMs
      ),
      printTimer_(clock_, Config::PrintIntervalMs)
{
}

void Application::setup()
{
    clock_.update();
    logger_.begin();
    potentiometer_.init();
    motorPwm_.init();
    uartExperiment_.init();
    logger_.log("Optional motor software PWM homework");
}

void Application::loop()
{
    clock_.update();

    if (potentiometer_.update())
    {
        motorPwm_.setDuty(potentiometer_.duty());
    }

    motorPwm_.update();
    uartExperiment_.update();

    if (printTimer_.ready())
    {
        logger_.logf(
            "ADC: %u | PWM: %u/255 (%u%%) | UART: 0x%02X\n",
            static_cast<unsigned>(potentiometer_.value()),
            static_cast<unsigned>(motorPwm_.duty()),
            static_cast<unsigned>(motorPwm_.duty()) * 100U / 255U,
            static_cast<unsigned>(uartExperiment_.currentPattern())
        );
    }
}
