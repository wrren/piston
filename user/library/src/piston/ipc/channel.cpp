#include <piston/ipc/channel.h>

namespace Piston::IPC
{
    Channel::Channel(Process::IDType target) :
    mTarget(target)
    {}

    Process::IDType Channel::GetTarget() const
    {
        return mTarget;
    }

    void Channel::Send(Message::PointerType Outgoing)
    {
        mOutbox.push_back(Outgoing);
    }

    Message::PointerType Channel::Receive()
    {
        if(mInbox.size())
        {
            auto Next = mInbox.front();
            mInbox.pop_front();

            return Next;
        }
        return Message::PointerType();
    }

    void Channel::PushMessage(Message::PointerType Message)
    {
        mInbox.push_back(Message);
    }

    Message::PointerType Channel::PopOutbox()
    {
        if(mOutbox.size())
        {
            auto Next = mOutbox.front();
            mOutbox.pop_front();

            return Next;
        }
        return Message::PointerType();
    }
}