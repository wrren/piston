#include <piston/core/log.h>

namespace Piston
{
    Log* Log::gLogInstance = nullptr;

    Log* Log::Instance()
    {
        if(gLogInstance == nullptr)
        {
            gLogInstance = new Log();
        }

        return gLogInstance;
    }

    void Log::Write(LevelType Level, const String& Message)
    {
        EntryType NewEntry{Level, Message, Time::GetTimestamp()};

        for(auto Backend : mBackends)
        {
            Backend->Write(NewEntry);
        }
    }

    void Log::AddBackend(Log::Backend::PointerType NewBackend)
    {
        mBackends.push_back(NewBackend);
    }

    void Log::RemoveBackend(Log::Backend::PointerType OldBackend)
    {
        mBackends.erase(std::remove(mBackends.begin(), mBackends.end(), OldBackend), mBackends.end());
    }
}