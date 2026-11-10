#include "Application.h"

#include <Arduino.h>

#include "Config.h"

Application::Application()
    : logger_(Serial, Config::SerialBaud),
      relay_(Config::RelayControlPin),
      contact_(
          Config::RelayFeedbackPin,
          Config::RelayDebounceUs
      ),
      relayTester_(
          relay_,
          contact_,
          Config::RelayMeasurementCount,
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
    logger_.log("Running one warm-up cycle...");
    logger_.log("Starting 10 ON and 10 OFF measurements...");
}

void Application::loop()
{
    const uint32_t nowUs = micros();
    const uint32_t nowMs = millis();

    RelayMeasurement measurement{};
    if (relayTester_.tick(nowUs, measurement))
    {
        reportMeasurement(measurement);
    }

    if (relayTester_.complete() && !summaryPrinted_)
    {
        summaryPrinted_ = true;
        completedAtMs_ = nowMs;
        logger_.logf(
            "Averages: ON = %lu us (%.3f ms), OFF = %lu us (%.3f ms)\n",
            static_cast<unsigned long>(relayTester_.averageOnUs()),
            relayTester_.averageOnUs() / 1000.0,
            static_cast<unsigned long>(relayTester_.averageOffUs()),
            relayTester_.averageOffUs() / 1000.0
        );
    }

    if (summaryPrinted_ &&
        nowMs - completedAtMs_ >= Config::RepeatIntervalMs)
    {
        summaryPrinted_ = false;
        errorPrinted_ = false;
        relayTester_.restart(nowUs);
        logger_.log("Restarting relay measurements...");
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
        measurement.transition == RelayTransition::On ? "ON" : "OFF",
        static_cast<unsigned>(measurement.number),
        static_cast<unsigned long>(measurement.durationUs),
        measurement.durationUs / 1000.0
    );
}
