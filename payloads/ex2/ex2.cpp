#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <Psapi.h>
#include <piston/core/format.h>
#include <piston/process/process.h>

#define THREAD_PRINT_OFFSET     0x1081
#define THREAD_SLEEP_OFFSET     0x1093
#define THREAD_RETURN_OFFSET    0x10a4

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

    piston::process::base_address parameter_address = base_address.value() + THREAD_PRINT_OFFSET;

    DebugPrint(piston::format("Rewriting Printf Parameter at ", std::hex, parameter_address, "\n").c_str());

    uint32_t new_parameter = 0;
    size_t bytes_written;

    if(!current_process->write_memory(parameter_address, &new_parameter, sizeof(new_parameter), bytes_written))
    {
        DebugPrint("Failed to write process memory.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    parameter_address = base_address.value() + THREAD_SLEEP_OFFSET;

    DebugPrint(piston::format("Rewriting Sleep Parameter at ", std::hex, parameter_address, "\n").c_str());

    new_parameter = 0;
    bytes_written;

    if(!current_process->write_memory(parameter_address, &new_parameter, sizeof(new_parameter), bytes_written))
    {
        DebugPrint("Failed to write process memory.\n");
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    parameter_address = base_address.value() + THREAD_RETURN_OFFSET;

    DebugPrint(piston::format("Rewriting Return Value at ", std::hex, parameter_address, "\n").c_str());

    new_parameter = 0;
    bytes_written;

    if(!current_process->write_memory(parameter_address, &new_parameter, sizeof(new_parameter), bytes_written))
    {
        DebugPrint("Failed to write process memory.\n");
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
