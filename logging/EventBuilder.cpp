#include "logging/EventBuilder.h"
#include "logging/Logger.h"


namespace logging
{
    EventBuilder::EventBuilder(Severity severity) :
        m_event{ severity, std::chrono::system_clock::now(), {} }
    {
    }

    EventBuilder::~EventBuilder()
    {
        Logger::get().sendEvent(m_event);
    }

    EventBuilder& EventBuilder::operator<<(const char* str)
    {
        m_event.message.append(str);
        return *this;
    }

    EventBuilder& EventBuilder::operator<<(const std::string& str)
    {
        m_event.message.append(str);
        return *this;
    }

} // namespace logging
