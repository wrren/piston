#ifndef PISTON_IPC_MESSAGE_FACTORY_H
#define PISTON_IPC_MESSAGE_FACTORY_H

#include <piston/core/types.h>
#include <piston/ipc/message.h>

namespace Piston::IPC
{
    class MessageFactory
    {
    public:

        typedef std::shared_ptr<MessageFactory> PointerType;
        typedef std::function<Message::PointerType ()> MessageMaker;

        /**
         * @brief Register a message maker function.
         * 
         * @param Command Command corresponding with the message to be created
         * @param Maker Message maker function
         */
        void RegisterMessageMaker(Message::CommandType Command, MessageMaker Maker);

        /**
         * @brief Determine whether the message factory has a registered message maker for the given command ID
         * 
         * @param Command Command
         * @return true If there's a registered message maker for this command
         * @return false Otherwise.
         */
        bool IsRegistered(Message::CommandType Command) const;

        /**
         * @brief Given an input stream, generate a message corresponding with the given message command
         * 
         * @param Command Message command ID
         * @param Stream Input stream
         * @return Message::PointerType 
         */
        Message::PointerType ToMessage(Message::CommandType Command, std::istream& Stream) const;

    private:

        // Message makers
        std::map<Message::CommandType, MessageMaker> mMakers;
    };
}

#endif // PISTON_IPC_MESSAGE_FACTORY_H