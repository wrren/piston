#include <piston/core/core.h>
#include <piston/process/injector.h>
#include <piston/process/process.h>
#include <wil/resource.h>
#include <Psapi.h>
#include <processthreadsapi.h>

namespace piston
{
    injector injector::inject(const injector::library_path& library_path, process::id_type process_id)
    {
        if(!std::filesystem::exists(library_path))
        {
            throw injector::injection_exception(format("Library file could not be found at path ", library_path));
        }

        char library_path_buf[_MAX_PATH];
        memset(library_path_buf, '\0', _MAX_PATH);
        
        if(strcpy_s(library_path_buf, _MAX_PATH, library_path.string().c_str()))
        {
            throw injector::injection_exception("Failed to allocate space for library path.");
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
        
        wil::unique_handle process_handle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, process_id));

        if(!process_handle)
        {
            throw injector::injection_exception(format("Failed to open a handle for process ", process_id));
        }

        LPVOID library_address = VirtualAllocEx(process_handle.get(), NULL, strnlen_s(library_path_buf, _MAX_PATH), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if(library_address == NULL)
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to allocate memory in target process for library address."));
        }

        if(!WriteProcessMemory(process_handle.get(), library_address, library_path_buf, strnlen_s(library_path_buf, _MAX_PATH), NULL))
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to write library path to target process memory."));
        }

        wil::unique_handle remote_thread_handle(CreateRemoteThread(process_handle.get(), NULL, 0, (LPTHREAD_START_ROUTINE) LoadLibraryA, library_address, 0, NULL));

        if(!remote_thread_handle)
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to create a remote thread in the target process."));
        }

        if(WaitForSingleObject(remote_thread_handle.get(), INFINITE))
        {
            VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);
            throw injector::injection_exception(format("Failed to wait for target thread to finish."));
        }

        VirtualFreeEx(process_handle.get(), library_address, 0, MEM_RELEASE);

        return injector(injector::mode::MODE_INJECT_RUNNING, library_path, process_id);
    }
    
    injector injector::inject(const injector::library_path& library_path, const injector::executable_path& executable_path, const injector::argument_list& arguments)
    {
        auto w_executable_path = convert::to_wstring(executable_path.string());
        auto w_current_directory = convert::to_wstring(executable_path.parent_path().string());
        auto w_command_line = convert::to_wstring(strings::join(arguments.begin(), arguments.end(), ' '));
        
        WCHAR w_command_line_buf[MAX_PATH];
        wcscpy_s(w_command_line_buf, MAX_PATH, w_command_line.c_str());
        
        STARTUPINFO startup_info = {0};
        startup_info.cb = sizeof(STARTUPINFO);

        PROCESS_INFORMATION process_info;

        if(!CreateProcess(w_executable_path.c_str(), w_command_line_buf, nullptr, nullptr, FALSE, 0, nullptr, w_current_directory.c_str(), &startup_info, &process_info))
        {
            throw injector::injection_exception("Failed to create process.");
        }

        return inject(library_path, process_info.dwProcessId);
    }
}