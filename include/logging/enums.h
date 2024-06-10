#pragma once
#include <string_view>


namespace logging
{
    enum class Severity : int
    {
        debug,
        info,
        warning,
        error,
        fatal
    };

    namespace enums
    {
        std::string_view toString(Severity severity);

        template<typename T>
        struct column_width
        {
            static const int value = 0;
        };

        template<>
        struct column_width<Severity>
        {
            static const int value = 7;
        };
    }
}
