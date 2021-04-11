#include <piston/ipc/messages/connect.h>

namespace Piston::IPC
{
    const Message::CommandType ConnectMessage::Command = Commands::CONNECT;

    ConnectMessage::ConnectMessage() :
    Message(ConnectMessage::Command)
    {}

    void ConnectMessage::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(ConnectMessage::Command, []()
        {
            return Message::PointerType(new ConnectMessage());
        });
    }

    bool ConnectMessage::Serialize(Stream& OutputStream) const
    {
        return true;
    }

    bool ConnectMessage::Deserialize(Stream& InputStream)
    {
        return true;
    }
}