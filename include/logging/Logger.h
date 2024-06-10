#pragma once
#include <memory>


namespace logging
{
    struct Event;
    class EventBuilder;

    namespace sinks { class Sink; }

    /*
     * Singleton class for managing the log events.
     * It should be configured with desired logging methods before first usage.
     * Any log event that is sent before the setup will be lost.
     * 
     * Setup options:
     * - one or more sinks that handle how the logs should be processed: sync/async
     * - provide to each sink a channel that sends log event data outside the application
     * - auto-flush (default: true): flush after each log event. Useful to have the latest logs in case the app crashes.
     */
    class Logger
    {
        Logger();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    public:
        ~Logger();

        static Logger& get();

        // Add a sync/async sink to manage a channel
        void addSink(std::unique_ptr<sinks::Sink>&& sink);

        // Set auto-flush for all channels
        void setAutoFlush(bool flag);

        // Flush all channels
        void flush();

    private:
        friend class ::logging::EventBuilder;
        void sendEvent(const Event& event);

        struct Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}
