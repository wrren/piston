#include <piston/ipc/router.h>
#include <piston/core/convert.h>
#include <piston/core/byte_stream.h>
#include <windows.h>

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

    std::vector<Message::PointerType> ReadMessages(HANDLE Pipe, MessageFactory::PointerType Factory)
    {
        std::vector<Message::PointerType> Messages;

        if(Pipe == NULL)
        {
            return Messages;
        }

        byte MessageBuffer[PIPE_BUFFER_SIZE];

        DWORD BytesRead;
        do
        {
            ReadFile(Pipe, MessageBuffer, PIPE_BUFFER_SIZE, &BytesRead, NULL);

            if(BytesRead == 0)
            {
                break;
            }

            ByteStream Stream(MessageBuffer, BytesRead);
            
            Process::IDType SourceID;
            Message::CommandType Command;

            Stream >> SourceID >> Command;

            if(Stream.fail())
            {
                break;
            }

            if(Factory->IsRegistered(Command))
            {
                auto NewMessage = Factory->ToMessage(Command, Stream);

                if(NewMessage)
                {
                    Messages.push_back(NewMessage);
                }
            }
        } while (GetLastError() != ERROR_NO_DATA);     

        return Messages;
    }

    bool SendMessage(HANDLE Pipe, Message::PointerType Message)
    {
        if(Pipe == NULL)
        {
            return false;
        }

        byte MessageBuffer[PIPE_BUFFER_SIZE];
        ByteStream Stream(MessageBuffer, PIPE_BUFFER_SIZE);

        Stream << Message->GetSourceProcessID() << Message->GetCommand();

        if(Message->Serialize(Stream))
        {
            DWORD BytesWritten;
            WriteFile(Pipe, MessageBuffer, static_cast<DWORD>(Stream.tellg()), &BytesWritten, NULL);
            return true;
        }
        return false;
    }

    void Router::Pump()
    {
        if(mHandle == nullptr)
        {
            mHandle = new PlatformHandle();
            mHandle->IncomingMessagePipe = MakeNamedPipe(mCurrentProcessID, mID);
        }

        auto IncomingMessages = ReadMessages(mHandle->IncomingMessagePipe, mMessageFactory);

        for(auto Message : IncomingMessages)
        {
            auto SourceProcessID = Message->GetSourceProcessID();
            auto SourceChannel = OpenChannel(SourceProcessID);
            SourceChannel->PushMessage(Message);
        }

        // Open new pipes for channels as needed
        for(auto& Entry : mChannels)
        {
            auto Channel = Entry.second;
            auto TargetProcessID = Channel->GetTarget();
            HANDLE Pipe;

            if(mHandle->OutgoingMessagePipes.find(TargetProcessID) == mHandle->OutgoingMessagePipes.end())
            {
                Pipe = MakeNamedPipe(TargetProcessID, mID);

                if(Pipe == INVALID_HANDLE_VALUE)
                {
                    continue;
                }

                mHandle->OutgoingMessagePipes[TargetProcessID] = Pipe;
            }
            else
            {
                Pipe = mHandle->OutgoingMessagePipes[TargetProcessID];
            }

            while(auto Message = Channel->PopOutbox())
            {
                Message->SetSourceProcessID(TargetProcessID);
                SendMessage(Pipe, Message);
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