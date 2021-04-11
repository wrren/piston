#ifndef PISTON_IPC_CONNECT_MESSAGE_H
#define PISTON_IPC_CONNECT_MESSAGE_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class ConnectMessage : public Message
    {
    public:

        // Connect Command ID
        static const Message::CommandType Command;

        /**
         * @brief Construct a new Connect Message
         * 
         */
        ConnectMessage();

        /**
         * @brief Register this message type with the given message factory.
         * 
         * @param Factory Message factory to register with.
         */
        static void RegisterWith(MessageFactory::PointerType Factory);

        /**
         * @brief Serialize a ConnectMessage into the given stream
         * 
         * @param OutputStream Output Stream
         * @return true If serialization was successful
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const override;

        /**
         * @brief Deserialize a ConnectMessage from the given stream
         * 
         * @param InputStream Input Stream
         * @return true If deserialization was successful
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;
    };
}

#endif // PISTON_IPC_CONNECT_MESSAGE_H