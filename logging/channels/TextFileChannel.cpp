#include "logging/channels/TextFileChannel.h"
#include <fstream>
#include "logging/channels/TextStreamChannel.h"


namespace logging
{
    namespace channels
    {
        struct TextFileChannel::Impl
        {
            std::ofstream fileStream;
            TextStreamChannel streamChannel;

            Impl(const std::string& filename) :
                fileStream{ filename, std::ofstream::app },
                streamChannel{ fileStream }
            {
            }

            // Any open file is automatically closed when the ofstream object is destroyed.
            // When the file is closed: before closing a file, all buffers that have not yet been flushed 
            // are synchronized and all pending data is written or read to the physical medium.
            ~Impl() = default;
        };

        TextFileChannel::TextFileChannel(const std::string& filename) :
            m_pImpl{ std::make_unique<Impl>(filename) }
        {
        }

        void TextFileChannel::write(const Event& event)
        {
            m_pImpl->streamChannel.write(event);
        }

        void TextFileChannel::flush()
        {
            m_pImpl->streamChannel.flush();
        }

        bool TextFileChannel::isOpen() const
        {
            return m_pImpl->fileStream.is_open();
        }
    }
}
