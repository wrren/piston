#ifndef PISTON_DRIVER_ENTRY_H
#define PISTON_DRIVER_ENTRY_H

#include <wdm.h>

extern "C" 
{
    /**
     * @brief Driver entry function
     * 
     * @param DriverObject Driver Object
     * @param RegistryPath Registry Path
     * @return NTSTATUS 
     */
    NTSTATUS DriverEntry(_Inout_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);

    /**
     * @brief Driver unload function
     * 
     * @param DriverObject Driver Object
     */
    void DriverUnload(_In_ PDRIVER_OBJECT DriverObject);
}

#endif // PISTON_DRIVER_ENTRY_H