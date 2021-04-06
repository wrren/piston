#ifndef PISTON_CORE_BYTE_STREAM_H
#define PISTON_CORE_BYTE_STREAM_H

#include <piston/core/types.h>

namespace Piston
{
    class ByteStream : public std::istream, std::ostream
    {
    public:

        class ByteBuffer  : public std::basic_streambuf<byte>
        {
        public:

            /**
             * @brief Construct a new Byte Buffer object
             * 
             * @param Data Data Buffer
             * @param Size Buffer Size
             */
            ByteBuffer(const byte* Data, size_t Size);
        };

        /**
         * @brief Construct a new Byte Stream object
         * 
         * @param Data Data Buffer
         * @param Size Buffer Size
         */
        ByteStream(const byte* Data, size_t Size);

    private:

        // Internal Buffer
        ByteBuffer mBuffer;
    };
}

#endif // PISTON_CORE_BYTE_STREAM_H