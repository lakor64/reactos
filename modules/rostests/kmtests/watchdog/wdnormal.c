/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         Kernel-mode test for normal watchdog routines
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wdioctl.h"
#include "wddpc.h"
#include <watchdog.h>

KMT_MESSAGE_HANDLER TestNormalWd;
NTSTATUS
TestNormalWd(
    _In_ PDEVICE_OBJECT DeviceObject,
    _In_ ULONG ControlCode,
    _In_opt_ PVOID Buffer,
    _In_ SIZE_T InLength,
    _Inout_ PSIZE_T OutLength
)
{
    PWATCHDOG pWd = NULL;
    PDPCCTX ctx = NULL;
    LARGE_INTEGER pTimeout;
    LARGE_INTEGER sysTime;
    PDEVICE_OBJECT wdObj;
    NTSTATUS s = STATUS_ABANDONED;
    ULONG lastEvt;

    UNREFERENCED_PARAMETER(ControlCode);
    UNREFERENCED_PARAMETER(Buffer);
    UNREFERENCED_PARAMETER(InLength);
    UNREFERENCED_PARAMETER(OutLength);
    
    pWd = WdAllocateWatchdog(DeviceObject, 
        WATCHDOG_MONITOR_USER_TIME,
        WATCHDOG_TEST_TAG);
    
    ok_bool_true(pWd != NULL, "Watchdog normal allocation");

    wdObj = WdGetDeviceObject(pWd);
    ok_eq_pointer(wdObj, DeviceObject);
    lastEvt = WdGetLastEvent(pWd);
    ok_eq_ulong(lastEvt, WATCHDOG_EVENT_CREATED);

    ctx = TestSetWdContextAndDpc();
    if (!ctx)
        return STATUS_NO_MEMORY;

    WdStartWatch(pWd, ctx->sDueTime, &ctx->dpc);

    pTimeout.QuadPart = 0;
    KeQueryTickCount(&sysTime);
    //while ((sysTime.QuadPart - ctx->sStartTime.QuadPart) <= WATCHDOG_NORMAL_MINIMUM_100NS_DUE_TIME_CHECK)
    while (1)
    {
        s = KeWaitForSingleObject(&ctx->pkEvent, Executive, KernelMode, FALSE, &pTimeout);
        if (s == STATUS_SUCCESS)
            break;
       KeQuerySystemTime(&sysTime);
    }

    ok_eq_ulong(s, STATUS_SUCCESS);
    lastEvt = WdGetLastEvent(pWd);
    ok_eq_ulong(lastEvt, WATCHDOG_EVENT_EXPIRED);
    KeClearEvent(&ctx->pkEvent);

    KeQuerySystemTime(&sysTime);
    while ((sysTime.QuadPart - ctx->sStartTime.QuadPart) <= WATCHDOG_NORMAL_MINIMUM_100NS_DUE_TIME_CHECK)
    {
        // we cannot directly call "KeWaitForSingleObject" as that pauses the thread time
        s = KeWaitForSingleObject(&ctx->pkEvent, Executive, KernelMode, FALSE, &pTimeout);
        if (s == STATUS_SUCCESS)
            break;
        KeQuerySystemTime(&sysTime);
    }
    ok_eq_ulong(s, STATUS_SUCCESS);

    lastEvt = WdGetLastEvent(pWd);
    ok_eq_ulong(lastEvt, WATCHDOG_EVENT_EXPIRED);
    KeClearEvent(&ctx->pkEvent);

    WdSuspendWatch(pWd);

    KeQuerySystemTime(&sysTime);
    while ((sysTime.QuadPart - ctx->sStartTime.QuadPart) <= WATCHDOG_NORMAL_MINIMUM_100NS_DUE_TIME_CHECK)
    {
        s = KeWaitForSingleObject(&ctx->pkEvent, Executive, KernelMode, FALSE, &pTimeout);
        KeQuerySystemTime(&sysTime);
    }

    ok_eq_ulong(s, STATUS_TIMEOUT);

    WdFreeWatchdog(pWd);
    ExFreePoolWithTag(ctx, WATCHDOG_TEST_TAG);

    return STATUS_SUCCESS;
}
