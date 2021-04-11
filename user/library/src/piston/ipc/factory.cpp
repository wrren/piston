#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    void MessageFactory::RegisterMessageMaker(Message::CommandType Command, MessageMaker Maker)
    {
        mMakers[Command] = Maker;
    }

    bool MessageFactory::IsRegistered(Message::CommandType Command) const
    {
        return mMakers.find(Command) != mMakers.end();
    }

    Message::PointerType MessageFactory::ToMessage(Message::CommandType Command, Stream& InputStream) const
    {
        auto Message = Message::PointerType();
        auto Entry = mMakers.find(Command);

        if(Entry != mMakers.end())
        {
            Message = Entry->second();
            
            if(Message->Deserialize(InputStream))
            {
                return Message;
            }
            
            Message.reset();
        }        

        return Message;
    }
}