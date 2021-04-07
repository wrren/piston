#include <piston/ipc/router.h>

namespace Piston::IPC
{
    Router::Router(Process::IDType CurrentProcessID, IDType RouterID) :
    mID(RouterID),
    mCurrentProcessID(CurrentProcessID),
    mHandle(nullptr),
    mMessageFactory(new MessageFactory())
    {}

    Channel::PointerType Router::OpenChannel(Process::IDType TargetProcess)
    {
        auto ExistingChannel = std::find_if(mChannels.begin(), mChannels.end(), [TargetProcess](std::pair<Process::IDType, Channel::PointerType> Entry)
        {
            return Entry.second->GetTarget() == TargetProcess;
        });

        if(ExistingChannel != mChannels.end())
        {
            return ExistingChannel->second;
        }

        auto NewChannel = Channel::PointerType(new Channel(TargetProcess));
        mChannels[TargetProcess] = NewChannel;

        for(auto Listener : mListeners)
        {
            Listener->OnChannelOpened(NewChannel);
        }

        return NewChannel;
    }

    std::vector<Channel::PointerType> Router::ListOpenChannels() const
    {
        std::vector<Channel::PointerType> Channels;

        for(auto Entry : mChannels)
        {
            Channels.push_back(Entry.second);
        }

        return Channels;
    }

    void Router::Broadcast(Message::PointerType Message)
    {
        for(auto& Entry : mChannels)
        {
            Entry.second->PushMessage(Message);
        }
    }

    MessageFactory::PointerType Router::GetMessageFactory() const
    {
        return mMessageFactory;
    }

    void Router::SetMessageFactory(MessageFactory::PointerType Factory)
    {
        mMessageFactory = Factory;
    }

    void Router::AddListener(Listener* NewListener)
    {
        mListeners.push_back(NewListener);
    }

    void Router::RemoveListener(Listener* ExistingListener)
    {
        mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), ExistingListener), mListeners.end());
    }
}