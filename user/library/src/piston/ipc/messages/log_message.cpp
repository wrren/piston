#include <piston/ipc/messages/log_message.h>

namespace Piston::IPC
{
    const Message::CommandType LogMessage::Command = 0;

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

    bool LogMessage::Deserialize(std::istream& Stream)
    {
        uint32_t MessageLength;
        Stream >> MessageLength;
        mMessage.reserve(MessageLength);
        Stream.read(mMessage.data(), MessageLength);

        return Stream.good();
    }

    bool LogMessage::Serialize(std::ostream& Stream) const
    {
        uint32_t MessageLength = static_cast<uint32_t>(mMessage.length());
        Stream << MessageLength;
        Stream.write(mMessage.c_str(), MessageLength);

        return Stream.good();
    }
}