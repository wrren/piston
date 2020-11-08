#include <piston/core/format.h>
#include <piston/process/injector.h>
#include <piston/process/process.h>
#include <wil/resource.h>
#include <Psapi.h>

namespace piston
{
    injector injector::inject(const injector::library_path& library_path, process::id_type process_id)
    {
        if(!std::filesystem::exists(library_path))
        {
            throw injector::injection_exception(format("Library file could not be found at path ", library_path));
        }

        auto list       = process::list();
        auto process    = std::find_if(list.begin(), list.end(), [process_id](process::ptr_type process_ptr)
        {
            return process_ptr->get_id() == process_id;
        });

        if(process == list.end())
        {
            throw injector::injection_exception(format("No process with ID matching ", process_id, " could be found"));
        }
        
        wil::unique_handle process_handle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id));

        if(!process_handle)
        {
            throw injector::injection_exception(format("Failed to open a handle for process ", process_id));
        }

        LPVOID library_address = VirtualAllocEx(process_handle.get(), NULL, _MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if(library_address == NULL)
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to allocate memory in target process for library address."));
        }

        if(!WriteProcessMemory(process_handle.get(), library_address, library_path.c_str(), library_path.string().length(), NULL))
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to write library path to target process memory."));
        }

        LPVOID load_library_address = GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryA");

        if(load_library_address == NULL)
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to get address for Kernel32::LoadLibraryA"));
        }

        wil::unique_handle remote_thread_handle(CreateRemoteThread(process_handle.get(), NULL, 0, (LPTHREAD_START_ROUTINE) load_library_address, library_address, 0, NULL));

        if(!remote_thread_handle)
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to create a remote thread in the target process."));
        }

        WaitForSingleObject(remote_thread_handle.get(), INFINITE);

        VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);

        return injector(injector::mode::MODE_INJECT_RUNNING, library_path, process_id);
    }
    
    injector injector::inject(const injector::library_path& library_path, const injector::executable_path& executable_path)
    {
        throw injector::injection_exception("Not implemented yet.");
    }
}