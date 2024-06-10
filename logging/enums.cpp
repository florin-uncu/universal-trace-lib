#include "logging/enums.h"


namespace logging
{
    namespace enums
    {
        std::string_view toString(Severity severity)
        {
            switch (severity)
            {
            case Severity::debug:   return "debug";
            case Severity::info:    return "info";
            case Severity::warning: return "warning";
            case Severity::error:   return "error";
            case Severity::fatal:   return "fatal";
            default:                return "unknown";
            }
        }
    }
}
