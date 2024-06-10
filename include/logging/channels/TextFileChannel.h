#pragma once
#include <memory>
#include <string>
#include "Channel.h"


namespace logging
{
    namespace channels
    {
        /*
         * This class writes log events to a text file using the same text format as TextStreamChannel.
         * If the file does not exist, then it will be created.
         * If the file already exists, then the logs will be appended at the end of the file.
         * On destructor:
         *  - the data is flushed
         *  - the file is closed
         */
        class TextFileChannel : public Channel
        {
        public:
            TextFileChannel(const std::string& filename);

            // Creates a text message with event data and writes it to provided file
            void write(const Event& event) override;

            // Calls flush on file stream
            void flush() override;

            // Returns true if the file is open and the log events can be written into it.
            bool isOpen() const;

        private:
            struct Impl;
            std::unique_ptr<Impl> m_pImpl;
        };
    }
}
