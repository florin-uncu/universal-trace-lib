cmake_minimum_required(VERSION 3.0)

project(UniversalTraceLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(HeaderFiles
	include/logging/log.h
	include/logging/enums.h
	include/logging/Event.h
	include/logging/EventBuilder.h
	include/logging/Logger.h
	include/logging/channels/Channel.h
	include/logging/channels/TextFileChannel.h
	include/logging/channels/TextStreamChannel.h
	include/logging/sinks/Sink.h
	include/logging/sinks/AsynchronousSink.h
	include/logging/sinks/SynchronousSink.h)

set(SourceFiles
	logging/enums.cpp
	logging/EventBuilder.cpp
	logging/Logger.cpp
	logging/channels/TextFileChannel.cpp
	logging/channels/TextStreamChannel.cpp
	logging/sinks/AsynchronousSink.cpp
	logging/sinks/SynchronousSink.cpp)

add_library(UniversalTrace STATIC ${HeaderFiles} ${SourceFiles})

# Demo project
add_executable(UniversalTraceLibDemo demo/main.cpp)
target_link_libraries(UniversalTraceLibDemo UniversalTrace)
