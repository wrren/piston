#include <piston/ipc/router.h>
#include <piston/core/convert.h>
#include <piston/core/byte_stream.h>
#include <piston/ipc/messages/connect.h>
#include <windows.h>

#define PIPE_BUFFER_SIZE 512000

namespace Piston::IPC
{
    struct Router::PlatformHandle
    {
        HANDLE IncomingMessagePipe;
        std::map<Process::IDType, HANDLE> OutgoingMessagePipes;
    };

    HANDLE MakeNamedPipe(Process::IDType TargetProcessID, Router::IDType RouterID, bool IsMine)
    {
        auto PipeName = std::wstring(L"\\\\.\\pipe\\piston-") + std::to_wstring(TargetProcessID) + L"-" + Convert::ToWideString(RouterID);

        PISTON_LOG_DEBUG("Creating named pipe ", Strings::WideStringToString(PipeName));

        if(IsMine)
        {
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
        else
        {
            auto Pipe = CreateFile(
                PipeName.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );

            if(Pipe != INVALID_HANDLE_VALUE)
            {
                DWORD PipeMode = PIPE_READMODE_MESSAGE;

                if(!SetNamedPipeHandleState(Pipe, &PipeMode, NULL, NULL))
                {
                    CloseHandle(Pipe);
                    return INVALID_HANDLE_VALUE;
                }
            }

            return Pipe;
        }        
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

            PISTON_LOG_DEBUG("Received ", BytesRead, " bytes from pipe.");

            ByteStream Stream(MessageBuffer, BytesRead);
            
            Process::IDType SourceID = 0;
            Message::CommandType Command = 0;


            Stream >> SourceID >> Command;
            
            PISTON_LOG_DEBUG("Message command ID: ", Command);

            if(!Stream.Good())
            {
                PISTON_LOG_ERROR("Stream in bad state on deserializing message header.");
                break;
            }

            if(Factory->IsRegistered(Command))
            {
                auto NewMessage = Factory->ToMessage(Command, Stream);

                if(NewMessage)
                {
                    NewMessage->SetSourceProcessID(SourceID);
                    
                    Messages.push_back(NewMessage);
                }
                else
                {
                    PISTON_LOG_ERROR("Failed to generate message for command ", Command);
                }
            }
            else
            {
                PISTON_LOG_ERROR("No register message maker for command ", Command);
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
            DWORD BytesToWrite = static_cast<DWORD>(Stream.GetPosition());

            PISTON_LOG_DEBUG("Writing ", BytesToWrite, " bytes to target pipe. Command ID: ", Message->GetCommand());

            WriteFile(Pipe, MessageBuffer, BytesToWrite, &BytesWritten, NULL);

            return true;
        }
        else
        {
            PISTON_LOG_ERROR("Failed to serialize message with command ", Message->GetCommand());
        }
        return false;
    }

    void Router::Pump()
    {
        if(mHandle == nullptr)
        {
            mHandle = new PlatformHandle();
            mHandle->IncomingMessagePipe = MakeNamedPipe(mCurrentProcessID, mID, true);

            if(mHandle->IncomingMessagePipe == NULL || mHandle->IncomingMessagePipe == INVALID_HANDLE_VALUE)
            {
                PISTON_LOG_ERROR("Failed to create incoming message pipe.");
            }
        }

        auto IncomingMessages = ReadMessages(mHandle->IncomingMessagePipe, mMessageFactory);

        for(auto Message : IncomingMessages)
        {
            PISTON_LOG_DEBUG("Received new message from process ", Message->GetSourceProcessID());

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
                Pipe = MakeNamedPipe(TargetProcessID, mID, false);

                if(Pipe == INVALID_HANDLE_VALUE || Pipe == NULL)
                {
                    PISTON_LOG_ERROR("Failed to create outgoing pipe for process ", TargetProcessID, ": ", GetLastError());
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
                PISTON_LOG_DEBUG("Sending message to process ", TargetProcessID);
                Message->SetSourceProcessID(mCurrentProcessID);
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