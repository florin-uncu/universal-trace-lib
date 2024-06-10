#include "logging/channels/TextStreamChannel.h"
#include <iomanip>
#include "logging/Event.h"


static std::string getCurrentDateTime(std::time_t t);

namespace logging
{
    namespace channels
    {
        TextStreamChannel::TextStreamChannel(std::ostream& outStream) :
            m_outStream{ outStream },
            m_isTimeIncluded{ true }
        {
        }

        TextStreamChannel::~TextStreamChannel()
        {
            flush();
        }

        void TextStreamChannel::write(const Event& event)
        {
            if (!m_outStream.good())
            {
                return;
            }

            if (m_isTimeIncluded)
            {
                auto t = std::chrono::system_clock::to_time_t(event.timePoint);
                m_outStream << getCurrentDateTime(t) << " | ";
            }

            m_outStream << std::setw(enums::column_width<Severity>::value) << enums::toString(event.severity) << " | ";
            m_outStream << event.message << '\n';
        }

        void TextStreamChannel::flush()
        {
            if (m_outStream.good())
            {
                m_outStream.flush();
            }
        }

        void TextStreamChannel::setTimeIncluded(bool flag)
        {
            m_isTimeIncluded = flag;
        }
    } // namespace channels
} // namespace logging

std::string getCurrentDateTime(std::time_t t)
{
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &t);
    std::string result{ buffer };
    // Remove new line at the end of the string
    result.pop_back();
    return result;
}
