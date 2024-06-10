#pragma once
#include <chrono>
#include <string>
#include "enums.h"

namespace logging
{
    struct Event
    {
        using TimePoint = std::chrono::system_clock::time_point;

        Severity severity;
        TimePoint timePoint;
        std::string message;
    };
}
