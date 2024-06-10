#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

// For LOG_INF, LOG_DBG defines
#include <logging/log.h>

// For initialization only
#include <logging/Logger.h>
#include <logging/channels/TextFileChannel.h>
#include <logging/channels/TextStreamChannel.h>
#include <logging/sinks/AsynchronousSink.h>
#include <logging/sinks/SynchronousSink.h>


// Used to create log file name
std::string getCurrentDate();

// Setup log channels
bool initalizeLogger();

// Called from multiple threads
void doAsyncWork(int tid)
{
    for (int i = 1; i <= 20; i++)
    {
        std::stringstream sstream;
        sstream << "thread " << tid << " (" << std::this_thread::get_id() << ") is working on job " << i << "...";
        LOG_DBG << sstream.str();
    }

    LOG_INF << "thread " << std::to_string(tid) << " finished work";
}

int main(int argc, char* argv[])
{
    if (!initalizeLogger())
    {
        std::cerr << "logger initialization failed" << std::endl;
        return 1;
    }

    LOG_INF << "Logger initialized";

    constexpr int ThreadsCount{ 10 };

    for (int i = 1; i < ThreadsCount; i++)
    {
        std::thread t{ &doAsyncWork, i };
        t.detach(); // thread may not finish its work when the process ends
    }

    std::thread t{ &doAsyncWork, ThreadsCount };
    t.join();

    std::cout << "end of main()" << std::endl;
    return 0;
}

std::string getCurrentDate()
{
    std::time_t t = std::time(0);
    std::tm now;
    localtime_s(&now, &t);
    return std::to_string(now.tm_year + 1900) + '-' +
        std::to_string(now.tm_mon + 1) + '-' +
        std::to_string(now.tm_mday);
}

bool initalizeLogger()
{
    using ConsoleChannel = logging::channels::TextStreamChannel;
    using TextFileChannel = logging::channels::TextFileChannel;
    using SyncSink = logging::sinks::SynchronousSink;
    using AsyncSink = logging::sinks::AsynchronousSink;

    // Logger singleton class
    auto& logger = logging::Logger::get();

    std::string logFileName = getCurrentDate() + ".txt";
    auto fileChannel = std::make_unique<TextFileChannel>(logFileName);
    if (!fileChannel->isOpen())
    {
        std::cerr << "Unable to open file: " << logFileName << std::endl;
        return false;
    }

    auto fileSink = std::make_unique<AsyncSink>(std::move(fileChannel));
    logger.addSink(std::move(fileSink));

    // Also show logs in console in debug mode
#ifdef _DEBUG
    auto consoleChannel = std::make_unique<ConsoleChannel>(std::cout);
    consoleChannel->setTimeIncluded(false);
    auto consoleSink = std::make_unique<SyncSink>(std::move(consoleChannel));
    logger.addSink(std::move(consoleSink));
#endif

    return true;
}
