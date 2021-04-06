#ifndef PISTON_PLATFORM_WINDOWS_TYPES_H
#define PISTON_PLATFORM_WINDOWS_TYPES_H

#include <windows.h>
#include <piston/core/types.h>

namespace Piston::Platform
{
    typedef DWORD ProcessID;

    typedef String ProcessName;

    typedef HANDLE ThreadID;
}

#endif // PISTON_PLATFORM_WINDOWS_TYPES_H