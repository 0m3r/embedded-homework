#include "Application.hpp"

namespace {
Application application;
}

void Application::init()
{
    fan_.begin();

    if (!timer_.init(Config::TimerTickMs, onTimer, this) ||
        !timer_.start()) {
        logger_.log("Hardware timer initialization failed\r\n");
        while (true) {}
    }

    logger_.log("Fan scheduler ready on PC15\r\n");
}

void Application::update()
{
    const auto events = fan_.takeEvents();

    for (std::uint32_t i = 0; i < events.starts; ++i) {
        logger_.log("Fan ON\r\n");
    }

    for (std::uint32_t i = 0; i < events.stops; ++i) {
        logger_.log("Fan OFF\r\n");
    }
}

void Application::onTimer(void* context)
{
    static_cast<Application*>(context)->fan_.onPeriod();
}

extern "C" void appInit()
{
    application.init();
}

extern "C" void appUpdate()
{
    application.update();
}
