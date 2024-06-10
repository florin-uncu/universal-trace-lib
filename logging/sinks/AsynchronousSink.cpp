#include "logging/sinks/AsynchronousSink.h"

#include <mutex>
#include <queue>
#include <thread>

#include "logging/Event.h"
#include "logging/channels/Channel.h"

#include <iostream>
namespace logging
{
    namespace sinks
    {
        /*
         * AsynchronousSink::Impl
         */
        struct AsynchronousSink::Impl
        {
            std::unique_ptr<channels::Channel> channel;
            std::queue<Event> eventsQueue;
            std::mutex eventsMutex;
            std::condition_variable eventsQueueCond;
            std::thread workerThread;
            bool isFlushNeeded{ false };
            bool isShutDownRequested{ false };

            ~Impl();
            void processEvents();
        };

        AsynchronousSink::Impl::~Impl()
        {
            {
                std::lock_guard lk(eventsMutex);
                isShutDownRequested = true;
            }

            eventsQueueCond.notify_one();
            
            if (workerThread.joinable())
            {
                workerThread.join();
            }
        }

        void AsynchronousSink::Impl::processEvents()
        {
            if (!channel)
            {
                return;
            }

            while (true)
            {
                std::unique_lock<std::mutex> lk{ eventsMutex };

                while (!eventsQueue.empty())
                {
                    const Event event = eventsQueue.front();
                    eventsQueue.pop();

                    // Queue unlocked while sending data
                    lk.unlock();
                    channel->write(event);
                    lk.lock();
                }

                if (isFlushNeeded)
                {
                    // Queue unlocked while flushing data
                    lk.unlock();
                    channel->flush();
                    lk.lock();
                    isFlushNeeded = false;
                }

                if (isShutDownRequested)
                {
                    break;
                }

                // Wait for changes in queue or flags
                eventsQueueCond.wait(lk);
            }
        }

        /*
         * AsynchronousSink
         */
        AsynchronousSink::AsynchronousSink(std::unique_ptr<channels::Channel>&& channel) : 
            m_pImpl{ std::make_unique<Impl>() }
        {
            m_pImpl->channel = std::move(channel);
            m_pImpl->workerThread = std::thread{ &Impl::processEvents, m_pImpl.get() };
        }

        void AsynchronousSink::write(const Event& event)
        {
            //TODO: maybe set a threshold for the size of this queue and 
            // keep an evidence of how many logs have been dropped
            {
                std::lock_guard lk(m_pImpl->eventsMutex);
                m_pImpl->eventsQueue.push(event);
            }

            m_pImpl->eventsQueueCond.notify_one();
        }

        void AsynchronousSink::flush()
        {
            // No lock_guard here
            // If the flag gets overwritten by worker thread, it means it doesn't need to flush anymore
            m_pImpl->isFlushNeeded = true;
            m_pImpl->eventsQueueCond.notify_one();
        }
    } // namespace sinks
} // namespace logging
