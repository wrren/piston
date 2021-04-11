#include <piston/core/core.h>
#include <piston/process/process.h>
#include <piston/platform/types.h>
#include <windows.h>
#include <Psapi.h>
#include <tlhelp32.h>

#define PISTON_MAX_PROCESSES 4096
#define PISTON_MAX_PROCESS_NAME_LENGTH	MAX_PATH

namespace Piston
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

	bool get_process_handle(Process::IDType id, DWORD desired_access, HANDLE* handle)
	{
		Platform::ProcessID process_ids[PISTON_MAX_PROCESSES];
        DWORD bytes_needed, num_processes;

		if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_needed))
		{
			throw Process::ListException();
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

	Process::PointerType Process::CurrentProcess()
	{
		auto list = Process::List();
		auto current_process_id = GetCurrentProcessId();

		for(auto process : list)
		{
			if(process->GetID() == current_process_id)
			{
				return process;
			}
		}
		return Process::PointerType();
	}

	std::optional<Process::AddressType> Process::GetBaseAddress() const
	{
		std::optional<Process::AddressType> result;
		std::wstring module_name;
		HANDLE process_handle;

		if(!get_process_handle(mID, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &process_handle))
		{
			return result;
		}

		if(!get_module_name(process_handle, module_name))
		{
			return result;
		}

		HANDLE module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, mID);

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
						result = reinterpret_cast<Process::AddressType>(module_entry.modBaseAddr);
					}
				} while(Module32Next(module_snapshot, &module_entry));
			}
		}
		CloseHandle(module_snapshot);

		return result;
	}

	std::vector<MemoryRegion> Process::ListMemoryRegions() const
	{
		std::vector<MemoryRegion> result;
		
		HANDLE process_handle;
		MEMORY_BASIC_INFORMATION memory_info;

		if(!get_process_handle(mID, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &process_handle))
		{
			return result;
		}

		LPCVOID start_address = 0;

		while(VirtualQueryEx(process_handle, start_address, &memory_info, sizeof(memory_info)))
		{
			if(memory_info.State == MEM_COMMIT && memory_info.Protect != PAGE_NOACCESS)
			{
				result.push_back(MemoryRegion(reinterpret_cast<MemoryRegion::AddressType>(memory_info.BaseAddress), memory_info.RegionSize));
			}

			start_address = (reinterpret_cast<byte*>(memory_info.BaseAddress) + memory_info.RegionSize);
		}

		return result;
	}

	Process::PointerType Process::FindByID(Process::IDType ProcessID)
	{
		PROCESSENTRY32 process_snapshot;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		process_snapshot.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(handle, &process_snapshot))
		{
			if (process_snapshot.th32ProcessID == ProcessID)
			{
				DWORD pid = process_snapshot.th32ProcessID;
				CloseHandle(handle);

				Process::NameType ProcessName(&process_snapshot.szExeFile[0]);

				return Process::PointerType(new Process(pid, ProcessName));
			}
		}

		CloseHandle(handle);

		return Process::PointerType();
	}

	Process::PointerType Process::FindByName(const Process::NameType& ProcessName)
	{	
		PROCESSENTRY32 process_snapshot;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		process_snapshot.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(handle, &process_snapshot))
		{
			if (!_wcsicmp(process_snapshot.szExeFile, ProcessName.c_str()))
			{
				DWORD pid = process_snapshot.th32ProcessID;
				CloseHandle(handle);

				return Process::PointerType(new Process(pid, ProcessName));
			}
		}

		CloseHandle(handle);

		return Process::PointerType();
	}

    Process::ListType Process::List()
    {
		Process::ListType results;
		Platform::ProcessID process_ids[PISTON_MAX_PROCESSES];
        DWORD bytes_needed, num_processes;
		TCHAR process_name[PISTON_MAX_PROCESS_NAME_LENGTH];

		if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_needed))
		{
			throw Process::ListException();
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
							Process::NameType Name(&process_name[0], &process_name[0] + wcslen(process_name));
							results.push_back(Process::PointerType(new Process(process_ids[i], Name)));
						}
					}
				}

				CloseHandle(process_handle);
			}
		}

		return results;
    }

	bool Process::ReadMemory(Process::AddressType address, void* buffer, size_t bytes, size_t& bytes_read)
	{
		HANDLE process_handle;
		if(!get_process_handle(mID, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &process_handle))
		{
			return false;
		}

		bool result = ReadProcessMemory(process_handle, (BYTE*) address, buffer, bytes, &bytes_read);
		CloseHandle(process_handle);

		return result;
	}

	bool Process::WriteMemory(Process::AddressType address, void* buffer, size_t bytes, size_t& bytes_written)
	{
		HANDLE process_handle;
		if(!get_process_handle(mID, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, &process_handle))
		{
			return false;
		}

		bool result = WriteProcessMemory(process_handle, (BYTE*) address, buffer, bytes, &bytes_written);
		CloseHandle(process_handle);

		return result;
	}

}