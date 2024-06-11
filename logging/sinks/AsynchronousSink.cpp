#include "logging/sinks/AsynchronousSink.h"

#include <atomic>
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
            std::atomic_bool isFlushNeeded{ false };
            std::atomic_bool isShutDownRequested{ false };

            ~Impl();
            void processEvents();
        };

        AsynchronousSink::Impl::~Impl()
        {
            isShutDownRequested = true;
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

                // Wait for changes in queue or flags
                eventsQueueCond.wait(lk, [this]() { return !eventsQueue.empty() || isFlushNeeded || isShutDownRequested; });

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
                    // Meanwhile a producer thread may add logs to the queue and set isFlushNeeded = true 
                    isFlushNeeded = false;
                    
                    lk.unlock();
                    channel->flush();
                }

                if (isShutDownRequested)
                {
                    break;
                }
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
            //TODO: Set a threshold for the max size of this queue and 
            // keep an evidence of how many logs have been dropped
            {
                std::lock_guard lk(m_pImpl->eventsMutex);
                m_pImpl->eventsQueue.push(event);
            }

            m_pImpl->eventsQueueCond.notify_one();
        }

        void AsynchronousSink::flush()
        {
            m_pImpl->isFlushNeeded = true;
            m_pImpl->eventsQueueCond.notify_one();
        }
    } // namespace sinks
} // namespace logging
