#pragma once
#include <string>
#include "Event.h"


namespace logging
{
    /*
     * Helper class for creating a log event.
     * It automatically sends the event when it is deleted.
     * If used as in example below, the instance will be deleted at the end of the statement.
     * 
     * Usage example:
     * #define LOG_DBG logging::EventBuilder{ logging::Severity::debug }
     * 
     * int a = 27;
     * LOG_DBG << __FILE__ << ":" << std::to_string(__LINE__) << " a = " << std::to_string(a);
     * 
     * LOG_DBG << "This is a multiline log:\n"
     *         << "Line 1\n"
     *         << "Line 2\n"
     *         << "Line 3";
     * 
     * Output:
     * - severity = debug
     * - timePoint = time when EventBuilder has been instantiated
     * - 1st log message = "ConsoleApplication.cpp:49 a = 27"
     * - 2nd log message = "This is a multiline log:\nLine 1\nLine2\Line3"
     */
    class EventBuilder
    {
    public:
        EventBuilder(Severity severity);
        EventBuilder(const EventBuilder&) = delete;
        EventBuilder& operator=(const EventBuilder&) = delete;
        ~EventBuilder();

        EventBuilder& operator<<(const char* str);
        EventBuilder& operator<<(const std::string& str);

    private:
        Event m_event;
    };
}
