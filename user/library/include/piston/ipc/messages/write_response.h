#ifndef PISTON_IPC_WRITE_RESPONSE_H
#define PISTON_IPC_WRITE_RESPONSE_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class WriteResponse : public Message
    {
    public:

        static const Message::CommandType Command;

        /**
         * @brief Construct a new Write Response object
         * 
         * @param Succeeded Indicates whether the request succeeded
         */
        WriteResponse(bool Succeeded = false);

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
         * @brief Determine whether the write request succeeded
         * 
         * @return true If the write request succeeded
         * @return false Otherwise
         */
        bool Succeeded() const;

    private:

        // Indicates whether the write operation was successful
        bool mSucceeded = false;
    };
}

#endif // PISTON_IPC_WRITE_RESPONSE_H