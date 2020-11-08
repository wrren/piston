#include <piston/process/process.h>
#include <piston/platform/types.h>
#include <windows.h>
#include <Psapi.h>

#define PISTON_MAX_PROCESSES 4096
#define PISTON_MAX_PROCESS_NAME_LENGTH	MAX_PATH

namespace piston
{
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
			}
		}

		return results;
    }
}