#include "logging/sinks/SynchronousSink.h"
#include <mutex>
#include "logging/channels/Channel.h"


namespace logging
{
    namespace sinks
    {
        struct SynchronousSink::Impl
        {
            std::unique_ptr<channels::Channel> channel;
            std::mutex channelMutex;
        };

        SynchronousSink::SynchronousSink(std::unique_ptr<channels::Channel>&& channel) :
            m_pImpl{ std::make_unique<Impl>() }
        {
            m_pImpl->channel = std::move(channel);
        }

        void SynchronousSink::write(const Event& event)
        {
            // If this is called from multiple threads, then make sure that this channel processes one event at a time
            std::lock_guard<std::mutex> lk(m_pImpl->channelMutex);
            if (m_pImpl->channel)
            {
                m_pImpl->channel->write(event);
            }
        }

        void SynchronousSink::flush()
        {
            std::lock_guard<std::mutex> lk(m_pImpl->channelMutex);
            if (m_pImpl->channel)
            {
                m_pImpl->channel->flush();
            }
        };
    }
}
