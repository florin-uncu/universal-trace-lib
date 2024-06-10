#include "logging/Logger.h"

#include <shared_mutex>
#include <vector>

#include "logging/Event.h"
#include "logging/sinks/Sink.h"


namespace logging
{
    using SinkPtr = std::unique_ptr<sinks::Sink>;

    struct Logger::Impl
    {
        std::vector<SinkPtr> sinksVect{};
        std::shared_mutex sinksVectMutex{};
        bool isAutoFlush{ true };
    };

    Logger::Logger() :
        m_pImpl{ std::make_unique<Impl>() }
    {
    }

    Logger::~Logger()
    {
        // Send all logs before closing the app
        flush();

        // Delay Logger's destruction until all threads finish processing their log event
        std::lock_guard lk(m_pImpl->sinksVectMutex);
        m_pImpl->sinksVect.clear();
    }

    Logger& Logger::get()
    {
        static Logger instance;
        return instance;
    }

    void Logger::addSink(std::unique_ptr<sinks::Sink>&& sink)
    {
        if (sink)
        {
            std::lock_guard lk(m_pImpl->sinksVectMutex);
            m_pImpl->sinksVect.push_back(std::move(sink));
        }
    }

    void Logger::setAutoFlush(bool flag)
    {
        m_pImpl->isAutoFlush = flag;
    }

    void Logger::flush()
    {
        // Allow multiple threads to call flush concurrently
        // Sink classes are thread safe
        std::shared_lock lk(m_pImpl->sinksVectMutex);

        for (SinkPtr& sink : m_pImpl->sinksVect)
        {
            if (sink)
            {
                sink->flush();
            }
        }
    }

    void Logger::sendEvent(const Event& event)
    {
        // Allow multiple threads to call Sink::write concurrently
        // Sink classes are thread safe
        std::shared_lock lk(m_pImpl->sinksVectMutex);

        for (SinkPtr& sink : m_pImpl->sinksVect)
        {
            if (sink)
            {
                sink->write(event);
                if (m_pImpl->isAutoFlush)
                {
                    sink->flush();
                }
            }
        }
    }

} // namespace logging
