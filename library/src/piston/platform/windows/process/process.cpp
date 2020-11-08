#include <piston/process/process.h>
#include <piston/platform/types.h>
#include <windows.h>

#define PISTON_MAX_PROCESSES 4096

namespace piston
{
    process::list_type process::list()
    {
        platform::process_id process_ids[PISTON_MAX_PROCESSES];
    }
}