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
      )
{
}

void Application::setup()
{
    logger_.begin();
    relayTester_.init(micros());

    logger_.log("Relay timing homework");
    logger_.log("Starting 10 ON and 10 OFF measurements...");
}

void Application::loop()
{
    relayTester_.update(micros());

    RelayMeasurement measurement{};
    if (relayTester_.takeMeasurement(measurement))
    {
        reportMeasurement(measurement);
    }

    if (relayTester_.complete() && !summaryPrinted_)
    {
        summaryPrinted_ = true;
        logger_.logf(
            "Averages: ON = %lu us (%.3f ms), OFF = %lu us (%.3f ms)\n",
            static_cast<unsigned long>(relayTester_.averageOnUs()),
            relayTester_.averageOnUs() / 1000.0,
            static_cast<unsigned long>(relayTester_.averageOffUs()),
            relayTester_.averageOffUs() / 1000.0
        );
    }

    if (relayTester_.failed() && !errorPrinted_)
    {
        errorPrinted_ = true;
        logger_.log("Timeout: check BC547B, relay and COM/NO wiring");
    }
}

void Application::reportMeasurement(const RelayMeasurement& measurement)
{
    logger_.logf(
        "Relay %s #%u: %lu us (%.3f ms)\n",
        measurement.transition == RelayTransition::TurnOn ? "ON" : "OFF",
        static_cast<unsigned>(measurement.number),
        static_cast<unsigned long>(measurement.durationUs),
        measurement.durationUs / 1000.0
    );
}
