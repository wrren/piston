#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <thread>
#include <Psapi.h>
#include <piston/core/format.h>
#include <piston/process/process.h>
#include <piston/ipc/router.h>
#include <piston/ipc/messages/log_message.h>

#define ROUTER_ID "scout-router"

bool IPCRunning = false;
std::thread IPCThread;

void DebugPrint(const char* msg)
{
    FILE* file = NULL;
    fopen_s(&file, "C:\\Users\\wkenny\\scout-log.txt", "a+");

    if(file != NULL)
    {
        fprintf(file, msg);
        fclose(file);
    }
}

void RunIPC()
{
    IPCRunning = true;

    DebugPrint("Starting IPC Thread");

    IPCThread = std::thread([]()
    {
        auto ThisProcess = Piston::Process::CurrentProcess();
        Piston::IPC::Router Router(ThisProcess->GetID(), ROUTER_ID);
        Piston::IPC::LogMessage::RegisterWith(Router.GetMessageFactory());
        
        DebugPrint("Starting Router Pump");

        while(IPCRunning)
        {
            Router.Broadcast(Piston::IPC::Message::PointerType(new Piston::IPC::LogMessage("Broadcast from inside target process!")));
            Router.Pump();
            DebugPrint("Router Pumped");
            std::this_thread::sleep_for(std::chrono::seconds(5));
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
    DebugPrint("DllMain Called");
    switch(Reason) {
    case DLL_PROCESS_ATTACH:
        RunIPC();
        break;
    case DLL_PROCESS_DETACH:
        // StopIPC();
        break;
    }

    return TRUE;   
}
