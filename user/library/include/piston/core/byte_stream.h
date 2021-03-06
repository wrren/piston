#ifndef PISTON_CORE_BYTE_STREAM_H
#define PISTON_CORE_BYTE_STREAM_H

#include <piston/core/stream.h>

namespace Piston
{
    /**
     * @brief Stream implementation that reads into, and writes from, a provided buffer.
     * 
     */
    class ByteStream : public Stream
    {
    public:
    
        /**
         * @brief Construct a new Byte Stream that will read from and write to the target buffer
         * 
         * @param Buffer Buffer
         * @param BufferSize Size of the buffer in bytes
         */
        ByteStream(byte* Buffer, PositionType BufferSize);

        /**
         * @brief Get the position representing the end of the stream.
         * 
         * @return PositionType 
         */
        virtual PositionType End() const override;

    protected:

        /**
         * @brief Write data into the stream at the current position.
         * 
         * @param Data Data to be written
         * @param DataSize Size of the data in bytes
         * @return true If the data was written successfully
         * @return false Otherwise.
         */
        virtual bool Write(const byte* Data, size_t DataSize) override;

        /**
         * @brief Read data from the stream at the current position
         * 
         * @param Data Destination buffer for the read data
         * @param DataSize Number of bytes to read
         * @return true If the data was read successfully
         * @return false Otherwise
         */
        virtual bool Read(byte* Data, size_t DataSize) const override;

    private:

        // Data buffer
        byte* mBuffer = nullptr;
        // Buffer size
        PositionType mSize = 0;
    };
}

#endif // PISTON_CORE_BYTE_STREAM_H