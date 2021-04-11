#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <thread>
#include <Psapi.h>
#include <Shlobj.h>
#include <piston/core/format.h>
#include <piston/process/process.h>
#include <piston/process/memory/scanner.h>
#include <piston/ipc/router.h>
#include <piston/ipc/messages/log.h>
#include <piston/ipc/messages/connect.h>
#include <piston/ipc/messages/scan_request.h>
#include <piston/ipc/messages/scan_response.h>
#include <piston/ipc/messages/write_request.h>
#include <piston/ipc/messages/write_response.h>
#include <piston/core/log/file_backend.h>

#define ROUTER_ID "scout-router"

bool IPCRunning = false;
std::thread IPCThread;

void RunIPC()
{
    IPCRunning = true;

    IPCThread = std::thread([]()
    {
        auto ThisProcess = Piston::Process::CurrentProcess();
        Piston::IPC::Router Router(ThisProcess->GetID(), ROUTER_ID);
        Piston::IPC::LogMessage::RegisterWith(Router.GetMessageFactory());
        Piston::IPC::ConnectMessage::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ScanRequest::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ScanResponse::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::WriteRequest::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::WriteResponse::RegisterWith(Router.GetMessageFactory());

        while(IPCRunning)
        {            
            Router.Pump();

            for(auto& Channel : Router.ListOpenChannels())
            {
                while(auto IncomingMessage = Channel->Receive())
                {
                    if(IncomingMessage->GetCommand() == Piston::IPC::Commands::SCAN_REQUEST)
                    {
                        PISTON_LOG_DEBUG("Received scan request");

                        auto Request = std::reinterpret_pointer_cast<Piston::IPC::ScanRequest>(IncomingMessage);
                        auto TargetRegions = ThisProcess->ListMemoryRegions();
                        Piston::MemoryBufferScanner Scanner(Request->GetTargetValue(), Request->GetTargetValueSize());
                        Piston::MemoryRegion::ListType Hits;

                        if(Request->GetScanType() == Piston::IPC::ScanRequest::ScanType::ReScan)
                        {
                            TargetRegions = Request->ListTargetRegions();
                        }
                        
                        for(auto& Region : TargetRegions)
                        {
                            std::unique_ptr<Piston::byte> ProcessData(new Piston::byte[Region.GetSize()]);
                            size_t BytesRead;

                            if(ThisProcess->ReadMemory(Region, ProcessData.get(), BytesRead))
                            {
                                for(auto& Hit : Scanner.Scan(ProcessData.get(), BytesRead))
                                {
                                    Hits.push_back(Piston::MemoryRegion(Region.GetStartAddress() + Hit, Request->GetTargetValueSize()));
                                }
                            }
                        }

                        Channel->Send(Piston::IPC::Message::PointerType(new Piston::IPC::ScanResponse(Hits)));
                    }
                    else if(IncomingMessage->GetCommand() == Piston::IPC::Commands::WRITE_REQUEST)
                    {

                        auto Request = std::reinterpret_pointer_cast<Piston::IPC::WriteRequest>(IncomingMessage);

                        PISTON_LOG_DEBUG("Received write request, target address: ", Request->GetTargetAddress());

                        size_t BytesWritten;

                        Channel->Send(Piston::IPC::Message::PointerType(
                            new Piston::IPC::WriteResponse(ThisProcess->WriteMemory(Request->GetTargetAddress(), Request->GetData(), Request->GetDataSize(), BytesWritten))
                        ));
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void StopIPC()
{
    IPCRunning = false;
    IPCThread.join();
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD Reason, LPVOID Reserved)
{
    char HomePathString[MAX_PATH];

    if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, HomePathString)))
    {
        Piston::Path HomePath(HomePathString);

        Piston::Log::Instance()->AddBackend(
            Piston::Log::Backend::PointerType(new Piston::FileBackend(HomePath / "scout.log"))
        );
    }

    switch(Reason) {
    case DLL_PROCESS_ATTACH:
        RunIPC();
        break;
    case DLL_PROCESS_DETACH:
        StopIPC();
        break;
    }

    return TRUE;   
}
