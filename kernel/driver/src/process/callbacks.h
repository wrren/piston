#ifndef PISTON_KERNEL_PROCESS_CALLBACKS_H
#define PISTON_KERNEL_PROCESS_CALLBACKS_H

#include <ntddk.h>

namespace piston
{
    /**
     * @brief Called when a process is created or destroyed
     * 
     * @param ParentId Parent process
     * @param ProcessId Process created or destroyed
     * @param Create Indicates whether the process was created or destroyed
     */
    void ProcessLifecycleCallback(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create);
}

#endif // PISTON_KERNEL_PROCESS_CALLBACKS_H