#pragma once
#include <istream>
#include "Channel.h"

namespace logging
{
    namespace channels
    {
        /*
         * This class writes log events to a std::ostream.
         * The written data is a human readable formatted text.
         * When destroyed the associated output stream flushes, but remains open.
         */
        class TextStreamChannel : public Channel
        {
        public:
            TextStreamChannel(std::ostream& outStream);
            ~TextStreamChannel() override;

            // Creates a text message with event data and writes it to associated stream
            void write(const Event& event) override;

            // Calls flush on associated stream
            void flush() override;

            // Flag for including the timestamp to the output text
            void setTimeIncluded(bool flag);

        private:
            std::ostream& m_outStream;
            bool m_isTimeIncluded;
        };
    }
}
