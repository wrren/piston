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

    void Channel::Send(const Message& message)
    {
        mOutbox.push_back(message);
    }

    bool Channel::Receive(Message& message)
    {
        if(mInbox.size())
        {
            message = mInbox.front();
            mInbox.pop_front();

            return true;
        }
        return false;
    }

    void Channel::PushMessage(const Message& message)
    {
        mInbox.push_back(message);
    }

    bool Channel::PopOutbox(Message& message)
    {
        if(mOutbox.size())
        {
            message = mOutbox.front();
            mOutbox.pop_front();

            return true;
        }
        return false;
    }
}