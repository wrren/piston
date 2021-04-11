#ifndef PISTON_IPC_SCAN_RESPONSE_H
#define PISTON_IPC_SCAN_RESPONSE_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class ScanResponse : public Message
    {
    public:

        static const Message::CommandType Command;

        /**
         * @brief Construct a new Scan Response object
         * 
         */
        ScanResponse();

        /**
         * @brief Construct a new Scan Response object
         * 
         * @param Results Scan results
         */
        ScanResponse(const MemoryRegion::ListType& Results);

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
         * @brief Get the list of scan results
         * 
         * @return MemoryRegion::ListType 
         */
        MemoryRegion::ListType GetResults() const;

    private:

        // Scan Results
        MemoryRegion::ListType mResults;
    };
}

#endif // PISTON_IPC_SCAN_RESPONSE_H