#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <Psapi.h>
#include <piston/core/format.h>
#include <piston/process/process.h>

#define THREAD_HANDLE_OFFSET 0x20BE0
#define THREAD_PRINT_OFFSET 0x1081

void DebugPrint(const char* msg)
{
    FILE* file = NULL;
    fopen_s(&file, "C:\\Users\\Warren\\Documents\\injector.txt", "a+");

    if(file != NULL)
    {
        fprintf(file, msg);
        fclose(file);
    }
}

LONG VectoredExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
    auto context = ExceptionInfo->ContextRecord;

    if(context)
    {
        ++(context->Rip);
    }

    auto current_process = piston::process::current_process();
    if(!current_process)
    {
        DebugPrint("Failed to get current process.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    auto base_address = current_process->get_base_address();
    if(!base_address)
    {
        DebugPrint("Failed to get process base address.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    DebugPrint(piston::format("Terminating Thread at ", std::hex, base_address.value() + THREAD_HANDLE_OFFSET, "\n").c_str());

    piston::process::base_address thread_address;
    size_t bytes_read;

    if(!current_process->read_memory(base_address.value() + THREAD_HANDLE_OFFSET, &thread_address, sizeof(thread_address), bytes_read))
    {
        DebugPrint("Failed to read process memory.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    HANDLE thread_handle = reinterpret_cast<HANDLE>(thread_address);
    if(!TerminateThread(thread_handle, 0))
    {
        DebugPrint("Failed to terminate thread.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    
    return EXCEPTION_CONTINUE_EXECUTION;
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD Reason, LPVOID Reserved)
{
    switch(Reason) {
    case DLL_PROCESS_ATTACH:
            AddVectoredExceptionHandler(1l, VectoredExceptionHandler);
            DebugPrint("DLL attach function called.\n");
        break;
    case DLL_PROCESS_DETACH:
                DebugPrint("DLL detach function called.\n");
        break;
    case DLL_THREAD_ATTACH:
                DebugPrint("DLL thread attach function called.\n");
        break;
    case DLL_THREAD_DETACH:
                DebugPrint("DLL thread detach function called.\n");
        break;
    }

    return TRUE;   
}
