#include <piston/core/core.h>
#include <piston/process/process.h>
#include <piston/platform/types.h>
#include <windows.h>
#include <Psapi.h>
#include <tlhelp32.h>

#define PISTON_MAX_PROCESSES 4096
#define PISTON_MAX_PROCESS_NAME_LENGTH	MAX_PATH

namespace piston
{
	bool get_module_name(HANDLE process, std::wstring& module_name)
	{
		DWORD buffer_size = 1024;
		TCHAR current_process_name[1024];
		if (!QueryFullProcessImageNameW(process, 0, current_process_name, &buffer_size))
		{
			return false;
		}

		module_name.assign(current_process_name, buffer_size);

		return true;
	}

	bool get_process_handle(process::id_type id, DWORD desired_access, HANDLE* handle)
	{
		platform::process_id process_ids[PISTON_MAX_PROCESSES];
        DWORD bytes_needed, num_processes;

		if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_needed))
		{
			throw process::list_exception();
		}

		num_processes = bytes_needed / sizeof(DWORD);

		for (DWORD i = 0; i < num_processes; i++)
		{
			if (process_ids[i] != 0 && process_ids[i] == id)
			{
				HANDLE process_handle = OpenProcess(desired_access, FALSE, process_ids[i]);

				if (process_handle != NULL)
				{
					*handle = process_handle;
					return true;
				}
			}
		}

		return false;
	}

	process::ptr_type process::current_process()
	{
		auto list = process::list();
		auto current_process_id = GetCurrentProcessId();

		for(auto process : list)
		{
			if(process->get_id() == current_process_id)
			{
				return process;
			}
		}
		return process::ptr_type();
	}

	std::optional<process::base_address> process::get_base_address() const
	{
		std::optional<process::base_address> result;
		std::wstring module_name;
		HANDLE process_handle;

		if(!get_process_handle(m_id, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &process_handle))
		{
			return result;
		}

		if(!get_module_name(process_handle, module_name))
		{
			return result;
		}

		HANDLE module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_id);

		if(module_snapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 module_entry;
			module_entry.dwSize = sizeof(MODULEENTRY32);

			if(Module32First(module_snapshot, &module_entry))
			{
				do
				{
					if(module_name == module_entry.szExePath)
					{
						result = reinterpret_cast<process::base_address>(module_entry.modBaseAddr);
					}
				} while(Module32Next(module_snapshot, &module_entry));
			}
		}
		CloseHandle(module_snapshot);

		return result;
	}

	process::ptr_type process::find_by_name(const process::name_type& name)
	{
		auto w_name = convert::to_wstring(name);
		
		PROCESSENTRY32 process_snapshot;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		process_snapshot.dwSize = sizeof(PROCESSENTRY32);

		do
		{
			if (!_wcsicmp(process_snapshot.szExeFile, w_name.c_str()))
			{
				DWORD pid = process_snapshot.th32ProcessID;
				CloseHandle(handle);

				return process::ptr_type(new process(pid, name));
			}
		} while (Process32Next(handle, &process_snapshot));

		CloseHandle(handle);

		return process::ptr_type();
	}

    process::list_type process::list()
    {
		process::list_type results;
		platform::process_id process_ids[PISTON_MAX_PROCESSES];
        DWORD bytes_needed, num_processes;
		TCHAR process_name[PISTON_MAX_PROCESS_NAME_LENGTH];

		if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_needed))
		{
			throw process::list_exception();
		}

		num_processes = bytes_needed / sizeof(DWORD);

		for (DWORD i = 0; i < num_processes; i++)
		{
			if (process_ids[i] != 0)
			{
				HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_ids[i]);

				if (process_handle != NULL)
				{
					HMODULE module_handle;
					if (EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &bytes_needed))
					{
						if (GetModuleBaseName(process_handle, module_handle, process_name, sizeof(process_name) / sizeof(TCHAR)))
						{
							results.push_back(process::ptr_type(new process(process_ids[i], (process::name_type::value_type*) process_name)));
						}
					}
				}

				CloseHandle(process_handle);
			}
		}

		return results;
    }

	bool process::read_memory(process::base_address address, void* buffer, size_t bytes, size_t& bytes_read)
	{
		HANDLE process_handle;
		if(!get_process_handle(m_id, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &process_handle))
		{
			return false;
		}

		bool result = ReadProcessMemory(process_handle, (BYTE*) address, buffer, bytes, &bytes_read);
		CloseHandle(process_handle);

		return result;
	}

	bool process::write_memory(process::base_address address, void* buffer, size_t bytes, size_t& bytes_written)
	{
		HANDLE process_handle;
		if(!get_process_handle(m_id, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, &process_handle))
		{
			return false;
		}

		bool result = WriteProcessMemory(process_handle, (BYTE*) address, buffer, bytes, &bytes_written);
		CloseHandle(process_handle);

		return result;
	}

}