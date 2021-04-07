#include <piston/ipc/messages/connect.h>

namespace Piston::IPC
{
    const Message::CommandType ConnectMessage::Command = 0;

    ConnectMessage::ConnectMessage() :
    Message(ConnectMessage::Command)
    {}

    bool ConnectMessage::Serialize(std::ostream& Stream) const
    {
        return true;
    }

    bool ConnectMessage::Deserialize(std::istream& Stream)
    {
        return true;
    }
}