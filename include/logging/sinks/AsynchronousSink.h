#pragma once
#include <memory>
#include "Sink.h"


namespace logging
{
    namespace channels { class Channel; }

    namespace sinks
    {
        /*
         * This class sends event logs asynchronously to avoid any delay to the application's flow.
         * The received logs are buffered if the channel is not fast enough.
         * This is a thread safe class of type: Multi-producers single consumer.
         */
        class AsynchronousSink : public Sink
        {
        public:
            AsynchronousSink(std::unique_ptr<channels::Channel>&& channel);

            void write(const Event& event) override;
            void flush() override;

        private:
            struct Impl;
            std::unique_ptr<Impl> m_pImpl;
        };
    }
}
