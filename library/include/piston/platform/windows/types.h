#ifndef PISTON_PLATFORM_WINDOWS_TYPES_H
#define PISTON_PLATFORM_WINDOWS_TYPES_H

#include <windows.h>
#include <piston/core/types.h>

namespace piston::platform
{
    typedef DWORD process_id;

    typedef string process_name;

    typedef HANDLE thread_id;
}

#endif // PISTON_PLATFORM_WINDOWS_TYPES_H