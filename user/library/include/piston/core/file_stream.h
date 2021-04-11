#ifndef PISTON_CORE_FILE_STREAM_H
#define PISTON_CORE_FILE_STREAM_H

#include <piston/core/stream.h>

namespace Piston
{
    class FileStream : public Stream
    {
    public:

        /**
         * @brief Construct a new File Stream object
         * 
         * @param FilePath Path to the underlying file
         * @param OpenMode File open mode
         */
        FileStream(const Path& FilePath, const char* OpenMode);

        /**
         * @brief Get the position representing the end of the stream.
         * 
         * @return PositionType 
         */
        virtual PositionType End() const override;

        /**
         * @brief Destroy the File Stream object
         * 
         */
        ~FileStream();

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

        // File Path
        Path mPath;
        // File Handle
        FILE* mFileHandle = nullptr;
    };
}

#endif // PISTON_CORE_FILE_STREAM_H