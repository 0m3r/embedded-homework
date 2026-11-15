#pragma once

#include "Button.hpp"
#include "Config.hpp"
#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/NoDebounceCounter.hpp"
#include "Debouncer/PollingDebouncer.hpp"
#include "Debouncer/StateBasedDebouncer.hpp"
#include "Debouncer/TimeBasedDebouncer.hpp"
#include "Logger.hpp"

class Application {
public:
    Application();
    void setup();
    void loop();

private:
    Logger logger_;
    Button button_{Config::ButtonPin};
    NoDebounceCounter noDebounce_{logger_};
    TimeBasedDebouncer timeBased_{logger_, Config::DebounceMs};
    StateBasedDebouncer stateBased_{logger_, Config::DebounceMs};
    PollingDebouncer polling_{logger_, Config::PollIntervalMs, Config::DebounceMs};
    IDebouncer* debouncers_[4]{
        &noDebounce_,
        &timeBased_,
        &stateBased_,
        &polling_,
    };
};
