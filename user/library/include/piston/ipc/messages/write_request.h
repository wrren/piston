#ifndef PISTON_IPC_WRITE_REQUEST_H
#define PISTON_IPC_WRITE_REQUEST_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class WriteRequest : public Message
    {
    public:

        static const Message::CommandType Command;

        /**
         * @brief Construct a new Write Request object
         * 
         */
        WriteRequest();

        /**
         * @brief Construct a new Write Request object
         * 
         * @param TargetAddress Target address for the write operation
         * @param Data Data to be written
         * @param DataSize Size of the data to be written
         */
        WriteRequest(MemoryRegion::AddressType TargetAddress, const byte* Data, size_t DataSize);

        /**
         * @brief Register this message type with the given message factory.
         * 
         * @param Factory Message factory to register with.
         */
        static void RegisterWith(MessageFactory::PointerType Factory);

        /**
         * @brief Deserialize a  message from the given input stream
         * 
         * @param InputStream Input stream
         * @return true If the message was deserialized successfully
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;

        /**
         * @brief Serialize a message into the given output stream
         * 
         * @param OutputStream Output stream
         * @return true If the message was serialized successfully
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const override;

        /**
         * @brief Get the Target Address for the write operation
         * 
         * @return MemoryRegion::AddressType 
         */
        MemoryRegion::AddressType GetTargetAddress() const;

        /**
         * @brief Get the Data to be written
         * 
         * @return const byte* 
         */
        byte* GetData() const;

        /**
         * @brief Get the Data Size
         * 
         * @return size_t 
         */
        size_t GetDataSize() const;

        /**
         * @brief Destroy the Write Request object
         * 
         */
        ~WriteRequest();

    private:

        // Target address for the write operation
        MemoryRegion::AddressType mTargetAddress = 0;
        // Data to be written
        byte* mData = nullptr;
        // Size of the data to be written
        size_t mDataSize = 0;
    };
}

#endif // PISTON_IPC_WRITE_REQUEST_H