#ifndef PISTON_IPC_LOG_MESSAGE_H
#define PISTON_IPC_LOG_MESSAGE_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class LogMessage : public Message
    {
    public:

        static const Message::CommandType Command;
        typedef String MessageType;

        /**
         * @brief Construct a new Log Message object
         * 
         */
        LogMessage();

        /**
         * @brief Construct a new Log Message object
         * 
         * @param Message Message String
         */
        LogMessage(const MessageType& Message);

        /**
         * @brief Get the LogMessage object
         * 
         * @return const MessageType& 
         */
        const MessageType& GetMessage() const;

        /**
         * @brief Register this message type with the given message factory.
         * 
         * @param Factory Message factory to register with.
         */
        static void RegisterWith(MessageFactory::PointerType Factory);

        /**
         * @brief Deserialize a log message from the given input stream
         * 
         * @param Stream Input stream
         * @return true If the message was deserialized successfully
         * @return false Otherwise
         */
        virtual bool Deserialize(std::istream& Stream) override;

        /**
         * @brief Serialize a log message into the given output stream
         * 
         * @param Stream Output stream
         * @return true If the message was serialized successfully
         * @return false Otherwise
         */
        virtual bool Serialize(std::ostream& Stream) const override;

    private:

        MessageType mMessage;
    };
}

#endif // PISTON_IPC_LOG_MESSAGE_H