#ifndef PISTON_IPC_MESSAGE_H
#define PISTON_IPC_MESSAGE_H

#include <piston/core/types.h>
#include <piston/process/process.h>

namespace Piston::IPC
{
    class Message : public Serializable
    {
    public:

        typedef uint16_t CommandType;
        typedef std::shared_ptr<Message> PointerType;

        /**
         * @brief Construct a new message object
         * 
         */
        Message() = default;

        /**
         * @brief Construct a new message object
         * 
         * @param command Command ID
         */
        Message(CommandType command);

        /**
         * @brief Get the Command ID of this message
         * 
         * @return CommandType 
         */
        CommandType GetCommand() const;

        /**
         * @brief Get the Source Process for this message
         * 
         * @return Process::IDType 
         */
        Process::IDType GetSourceProcessID() const;

        /**
         * @brief Set the Source Process ID for this message
         * 
         * @param ID Source Process ID
         */
        void SetSourceProcessID(Process::IDType ID);

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param stream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(std::istream& stream) = 0;

        /**
         * @brief Serialize data into the given stream
         * 
         * @param stream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool Serialize(std::ostream& stream) const = 0;

    private:

        // Message command
        CommandType mCommand;
        // Source process ID
        Process::IDType mProcessID;
    };
}

#endif // PISTON_IPC_MESSAGE_H