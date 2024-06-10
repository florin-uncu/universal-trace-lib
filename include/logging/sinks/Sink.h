#pragma once

namespace logging
{
    struct Event;

    namespace sinks
    {
        /*
         * This class is responsible of forwarding log events to an associated channel.
         * A derived class can choose how to handle log events before forwarding them to the channel.
         * The implementation of Sink interface shall be thread safe.
         * It is used by Logger class to write log events to one or more channels at the same time.
         */
        class Sink
        {
        public:
            virtual ~Sink() = default;

            virtual void write(const Event& event) = 0;
            virtual void flush() = 0;
        };
    }
}
