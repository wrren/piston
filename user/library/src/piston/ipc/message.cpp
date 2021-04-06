#include <piston/ipc/message.h>

namespace Piston::IPC
{
    Message::Message(CommandType command) :
    mCommand(command)
    {}

    Message::CommandType Message::GetCommand() const
    {
        return mCommand;
    }
}