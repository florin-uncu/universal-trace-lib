#pragma once
#include <memory>
#include "Sink.h"


namespace logging
{
    namespace channels { class Channel; }

    namespace sinks
    {
        /*
         * This class sends event logs synchronously so it may cause some delay to the application's flow.
         * This is a thread safe class.
         */
        class SynchronousSink : public Sink
        {
        public:
            SynchronousSink(std::unique_ptr<channels::Channel>&& channel);

            void write(const Event& event) override;
            void flush() override;

        private:
            struct Impl;
            std::unique_ptr<Impl> m_pImpl;
        };
    }
}
