/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         Kernel-mode test for deferred watchdog routines
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wdioctl.h"
#include <watchdog.h>

KMT_MESSAGE_HANDLER TestDeferredWd;
NTSTATUS
TestDeferredWd(
    _In_ PDEVICE_OBJECT DeviceObject,
    _In_ ULONG ControlCode,
    _In_opt_ PVOID Buffer,
    _In_ SIZE_T InLength,
    _Inout_ PSIZE_T OutLength
)
{
    //PWATCHDOG pWd;
    //KDPC pDpc;

    UNREFERENCED_PARAMETER(ControlCode);
    UNREFERENCED_PARAMETER(Buffer);
    UNREFERENCED_PARAMETER(InLength);
    UNREFERENCED_PARAMETER(OutLength);
    UNREFERENCED_PARAMETER(DeviceObject);

#if 0

    pWd = WdAllocateDeferredWatchdog(
        (PDEVICE_OBJECT)DriverObject,
        WATCHDOG_MONITOR_KERNEL_TIME,
        (ULONG)'stmK'
    );

    ok_bool_true(pWd != NULL, "Watchdog deferred allocation");

    WdStartDeferredWatch(pWd, &pDpc, 100);

    while (1)
    {
        KeQueryTickCount(&sTick);

        if (sTick.QuadPart >= (sCtx.sStartTime.QuadPart + 200))
            break;
    }

    ok_bool_false(sCtx.bOk, "Dpc was not called");

    KeQueryTickCount(&sCtx.sStartTime);
    WdEnterMonitoredSection(pWd);

    while (1)
    {
        KeQueryTickCount(&sTick);

        if (sTick.QuadPart >= (sCtx.sStartTime.QuadPart + 200))
            break;
    }

    ok_bool_false(sCtx.bOk, "Dpc was called");

    WdExitMonitoredSection(pWd);
    WdFreeDeferredWatchdog(pWd);
#endif

    return STATUS_SUCCESS;
}
