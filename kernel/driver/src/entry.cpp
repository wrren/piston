#include <ntifs.h>
#include <wdm.h>
#include <ntddk.h>
#include "entry.h"
#include "process/callbacks.h"

extern "C"
{
    NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
    {
        UNREFERENCED_PARAMETER(RegistryPath);

        DriverObject->DriverUnload = DriverUnload;

        PsSetCreateProcessNotifyRoutine(piston::ProcessLifecycleCallback, FALSE);

        return STATUS_SUCCESS;
    }

    void DriverUnload(PDRIVER_OBJECT DriverObject)
    {
        PsSetCreateProcessNotifyRoutine(piston::ProcessLifecycleCallback, TRUE);
        return;
    }
}