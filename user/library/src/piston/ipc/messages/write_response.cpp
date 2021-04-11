#include <piston/ipc/messages/write_response.h>

namespace Piston::IPC
{
    const Message::CommandType WriteResponse::Command = Commands::WRITE_RESPONSE;

    WriteResponse::WriteResponse(bool Succeeded) :
    Message(Command),
    mSucceeded(Succeeded)
    {}

    void WriteResponse::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(WriteResponse::Command, []()
        {
            return Message::PointerType(new WriteResponse());
        });
    }

    bool WriteResponse::Deserialize(Stream& InputStream)
    {
        return (InputStream >> mSucceeded).Good();
    }

    bool WriteResponse::Serialize(Stream& OutputStream) const
    {
        return (OutputStream << mSucceeded).Good();
    }

    bool WriteResponse::Succeeded() const
    {
        return mSucceeded;
    }
}