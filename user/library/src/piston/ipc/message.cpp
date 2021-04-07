#include <piston/ipc/message.h>

namespace Piston::IPC
{
    Message::Message(CommandType Command) :
    mCommand(Command)
    {}

    Message::CommandType Message::GetCommand() const
    {
        return mCommand;
    }

    Process::IDType Message::GetSourceProcessID() const
    {
        return mProcessID;
    }

    void Message::SetSourceProcessID(Process::IDType ID)
    {
        mProcessID = ID;
    }
}