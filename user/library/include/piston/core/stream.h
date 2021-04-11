#ifndef PISTON_CORE_STREAM_H
#define PISTON_CORE_STREAM_H

#include <piston/core/types.h>
#include <type_traits>

namespace Piston
{
    class Stream
    {
    public:

        typedef size_t PositionType;

        /**
         * @brief Get the current stream position.
         * 
         * @return PositionType 
         */
        PositionType GetPosition() const;

        /**
         * @brief Seek to the specified position within the stream. This operation
         * may cause the stream's Good state to change.
         * 
         * @param NewPosition New Position
         * @return Stream& 
         */
        Stream& Seek(PositionType NewPosition);

        /**
         * @brief Get the position representing the end of the stream.
         * 
         * @return PositionType 
         */
        virtual PositionType End() const = 0;

        /**
         * @brief Determine whether the stream is in a good state. Indicates whether prior operations
         * succeeded and whether subsequent ones can succeed.
         * 
         * @return true If the stream is in a good state
         * @return false Otherwise.
         */
        bool Good() const;

        /**
         * @brief Serialize a string into this stream
         * 
         * @param InputString String to be serialized
         * @return Stream& This stream
         */
        Stream& operator<<(const char* InputString);

        /**
         * @brief Serialize a string into this stream
         * 
         * @param InputString String to be serialized
         * @return Stream& This stream
         */
        Stream& operator<<(const String& InputString);

        /**
         * @brief Deserialize a string from this stream
         * 
         * @param OutputString Stream to be deserialized
         * @return Stream& This stream
         */
        Stream& operator>>(String& OutputString);

        /**
         * @brief Serialize a POD-type value into the stream
         * 
         * @tparam T POD Type
         * @param InputValue Value to be serialized
         * @return Stream& This stream
         */
        template<typename T, std::enable_if_t<std::is_pod<T>::value, bool> = true>
        Stream& operator<<(T InputValue)
        {
            WriteBuffer(reinterpret_cast<const byte*>(&InputValue), sizeof(T));
            return *this;
        }

        /**
         * @brief Deserialize a POD-type value from the stream
         * 
         * @tparam T POD Type
         * @param OutputValue Value to be deserialized
         * @return Stream& This stream
         */
        template<typename T, std::enable_if_t<std::is_pod<T>::value, bool> = true>
        Stream& operator>>(T& OutputValue)
        {
            ReadBuffer(reinterpret_cast<byte*>(&OutputValue), sizeof(T));
            return *this;
        }

        /**
         * @brief Serialize a list of values into the stream
         * 
         * @tparam T 
         * @param InputValue List to be serialized
         * @return Stream& This stream
         */
        template<typename T>
        Stream& operator<<(const std::vector<T>& InputValue)
        {
            auto ListSize = static_cast<uint32_t>(InputValue.size());

            (*this) << ListSize;

            for(auto& Element : InputValue)
            {
                (*this) << Element;
            }

            return *this;
        }

        /**
         * @brief Deserialize a list of values from the stream
         * 
         * @tparam T 
         * @param OutputValue List to be deserialized
         * @return Stream& This stream
         */
        template<typename T>
        Stream& operator>>(std::vector<T>& OutputValue)
        {
            uint32_t ListSize;

            (*this) >> ListSize;

            for(uint32_t i = 0; i < ListSize; i++)
            {
                T Element;
                (*this) >> Element;
                OutputValue.push_back(Element);
            }

            return *this;
        }

        /**
         * @brief Write a data buffer to the stream
         * 
         * @param Data Data buffer
         * @param DataSize Size of the data buffer in bytes
         * @return true If the write was successful
         * @return false Otherwise
         */
        bool WriteBuffer(const byte* Data, size_t DataSize);

        /**
         * @brief Read a data buffer from the stream
         * 
         * @param Data Destination data buffer
         * @param DataSize Number of bytes to read from the stream
         * @return true If the read was successful
         * @return false Otherwise
         */
        bool ReadBuffer(byte* Data, size_t DataSize);

    protected:

        /**
         * @brief Write data into the stream at the current position.
         * 
         * @param Data Data to be written
         * @param DataSize Size of the data in bytes
         * @return true If the data was written successfully
         * @return false Otherwise.
         */
        virtual bool Write(const byte* Data, size_t DataSize) = 0;

        /**
         * @brief Read data from the stream at the current position
         * 
         * @param Data Destination buffer for the read data
         * @param DataSize Number of bytes to read
         * @return true If the data was read successfully
         * @return false Otherwise
         */
        virtual bool Read(byte* Data, size_t DataSize) const = 0;

    private:

        // Current stream position
        PositionType mPosition = 0;
        // Current State
        bool mGood = true;
    };
}

#endif // PISTON_CORE_STREAM_H