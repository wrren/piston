#include <piston/ipc/router.h>

namespace Piston::IPC
{
    Router::Router(Process::IDType CurrentProcessID, IDType RouterID) :
    mID(RouterID),
    mCurrentProcessID(CurrentProcessID),
    mHandle(nullptr)
    {}

    Channel::PointerType Router::OpenChannel(Process::IDType TargetProcess)
    {
        auto ExistingChannel = std::find_if(mChannels.begin(), mChannels.end(), [TargetProcess](Channel::PointerType Channel)
        {
            return Channel->GetTarget() == TargetProcess;
        });

        if(ExistingChannel != mChannels.end())
        {
            return *ExistingChannel;
        }

        auto NewChannel = Channel::PointerType(new Channel(TargetProcess));
        mChannels.push_back(NewChannel);

        return NewChannel;
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