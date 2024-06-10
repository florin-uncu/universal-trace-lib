#pragma once

namespace logging
{
    struct Event;

    namespace channels
    {
        /*
         * Interface for writing log events outside application.
         * Example of channels: console, file, TCP/IP, serial etc.
         * The derived class is expected to flush on destructor.
         */
        class Channel
        {
        public:
            virtual ~Channel() = default;

            //TODO: discussion: Maybe it's better to make this class more basic (e.g., write(const char* data) )
            // and move serialization of the Event into a dedicated class.
            // With current design, for example, if we need to support writing log events to TCP/IP with multiple serializations (JSON, XML)
            // we need to create a TcpChannel initialized with a parameter for serialization type
            virtual void write(const Event& event) = 0;

            virtual void flush() = 0;
        };
    }
}
