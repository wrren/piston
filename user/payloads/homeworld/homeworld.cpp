#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <Psapi.h>
#include <piston/core/format.h>
#include <piston/process/process.h>

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

BOOL APIENTRY DllMain(HMODULE hDll, DWORD Reason, LPVOID Reserved)
{
    switch(Reason) {
    case DLL_PROCESS_ATTACH:
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
