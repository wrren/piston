#include <piston/ipc/router.h>
#include <windows.h>
#include <piston/core/convert.h>
#define PIPE_BUFFER_SIZE 8192

namespace Piston::IPC
{
    struct Router::PlatformHandle
    {
        HANDLE IncomingMessagePipe;
        std::map<Process::IDType, HANDLE> OutgoingMessagePipes;
    };

    HANDLE MakeNamedPipe(Process::IDType TargetProcessID, Router::IDType RouterID)
    {
        auto PipeName = std::wstring(L"\\\\.\\pipe\\piston-") + std::to_wstring(TargetProcessID) + L"-" + Convert::ToWideString(RouterID);
        return CreateNamedPipe(
            PipeName.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
            PIPE_UNLIMITED_INSTANCES,
            PIPE_BUFFER_SIZE,
            PIPE_BUFFER_SIZE,
            0,
            NULL
        );
    }

    void Router::Pump()
    {
        if(mHandle == nullptr)
        {
            mHandle = new PlatformHandle();
            mHandle->IncomingMessagePipe = MakeNamedPipe(mCurrentProcessID, mID);
        }

        // Open new pipes for channels as needed
        for(auto& Channel : mChannels)
        {
            auto TargetProcessID = Channel->GetTarget();

            if(mHandle->OutgoingMessagePipes.find(TargetProcessID) == mHandle->OutgoingMessagePipes.end())
            {
                auto NewHandle = MakeNamedPipe(TargetProcessID, mID);

                if(NewHandle == INVALID_HANDLE_VALUE)
                {
                    continue;
                }

                mHandle->OutgoingMessagePipes[TargetProcessID] = NewHandle;
            }
        }        
    }

    Router::~Router()
    {
        if(mHandle != nullptr)
        {
            CloseHandle(mHandle->IncomingMessagePipe);

            for(auto it : mHandle->OutgoingMessagePipes)
            {
                CloseHandle(it.second);
            }
            delete mHandle;
            mHandle = nullptr;
        }
    }
}