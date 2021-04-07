#ifndef PISTON_IPC_CHANNEL_H
#define PISTON_IPC_CHANNEL_H

#include <piston/process/process.h>
#include <piston/ipc/message.h>

namespace Piston::IPC
{
    class Channel
    {
    public:

        typedef std::shared_ptr<Channel> PointerType;

        /**
         * @brief Send a message to the target process
         * 
         * @param Message Message to be sent
         * @return true If the message was sent successfully
         * @return false Otherwise
         */
        void Send(Message::PointerType Message);

        /**
         * @brief Receive a message from the channel's inbox
         * 
         * @param Message Mesage to be received
         * @return true If there was a message in the inbox to be received
         *         false Otherwise
         */
        Message::PointerType Receive();

        /**
         * @brief Get the target process for this channel
         * 
         * @return process::ptr_type 
         */
        Process::IDType GetTarget() const;

    protected:

        /**
         *  Open an IPC channel for communication with the target process.
         */
        Channel(Process::IDType target);

        /**
         * @brief Push a new message onto this channel's inbox.
         * 
         * @param Message New message
         */
        void PushMessage(Message::PointerType Message);

        /**
         * @brief Pop a message off the channel's outbox
         * 
         * @param Message Output message
         * @return true If there were messages to be popped
         *         false Otherwise
         */
        Message::PointerType PopOutbox();

    private:

        // Allow router to access our message queues.
        friend class Router;

        // Target Process ID
        Process::IDType mTarget;
        // Outgoing message queue
        std::deque<Message::PointerType> mOutbox;
        // Incoming message queue
        std::deque<Message::PointerType> mInbox;
    };
}

#endif // PISTON_IPC_CHANNEL_H