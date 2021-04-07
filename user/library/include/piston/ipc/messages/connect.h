#ifndef PISTON_IPC_CONNECT_MESSAGE_H
#define PISTON_IPC_CONNECT_MESSAGE_H

#include <piston/ipc/message.h>

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
         * @brief Serialize a ConnectMessage into the given stream
         * 
         * @param Stream Output Stream
         * @return true If serialization was successful
         * @return false Otherwise
         */
        virtual bool Serialize(std::ostream& Stream) const override;

        /**
         * @brief Deserialize a ConnectMessage from the given stream
         * 
         * @param Stream Input Stream
         * @return true If deserialization was successful
         * @return false Otherwise
         */
        virtual bool Deserialize(std::istream& Stream) override;
    };
}

#endif // PISTON_IPC_CONNECT_MESSAGE_H