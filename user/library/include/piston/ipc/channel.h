#ifndef PISTON_IPC_CHANNEL_H
#define PISTON_IPC_CHANNEL_H

#include <piston/process/process.h>
#include <piston/ipc/message.h>

namespace piston::ipc
{
    class channel
    {
    public:

        typedef std::shared_ptr<channel> ptr_type;

        /**
         *  Open an IPC channel for communication with the target process.
         */
        channel(process::ptr_type target);

        /**
         * @brief  Opens a channel for communication with the target process.
         * 
         * @return true If the channel was opened.
         * @return false Otherwise.
         */
        bool open();

        /**
         * @brief Send a message to the target process
         * 
         * @param message Message to be sent
         * @return true If the message was sent successfully
         * @return false Otherwise
         */
        bool send(const message& message);

        /**
         * @brief Receive pending messages from the channel
         * 
         * @return std::vector<message> 
         */
        std::vector<message> receive();

        /**
         * @brief Destroy this channel object
         * 
         */
        ~channel();

    private:

        process::ptr_type m_target;
    };
}

#endif // PISTON_IPC_CHANNEL_H