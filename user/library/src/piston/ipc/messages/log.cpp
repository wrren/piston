#include <piston/ipc/messages/log.h>

namespace Piston::IPC
{
    const Message::CommandType LogMessage::Command = Commands::LOG_MESSAGE;

    LogMessage::LogMessage() :
    Message(Command)
    {}

    LogMessage::LogMessage(const LogMessage::MessageType& MessageString) :
    Message(Command),
    mMessage(MessageString)
    {}

    const LogMessage::MessageType& LogMessage::GetMessage() const
    {
        return mMessage;
    }

    void LogMessage::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(LogMessage::Command, []()
        {
            return Message::PointerType(new LogMessage());
        });
    }

    bool LogMessage::Deserialize(Stream& InputStream)
    {
        InputStream >> mMessage;
        return InputStream.Good();
    }

    bool LogMessage::Serialize(Stream& OutputStream) const
    {
        OutputStream << mMessage;
        return OutputStream.Good();
    }
}