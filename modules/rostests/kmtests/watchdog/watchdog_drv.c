
#include <kmt_test.h>

//#define NDEBUG
#include <debug.h>
#include <watchdog.h>

struct DpcCtx
{
    LARGE_INTEGER sStartTime;
    BOOLEAN bOk;
};

void KdeferredRoutine(
  struct _KDPC  *Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2
)
{
    struct DpcCtx* ctx = (struct DpcCtx*)DeferredContext;
    LARGE_INTEGER sCur;
    KeQueryTickCount(&sCur);
    ok_bool_true((sCur.QuadPart - ctx->sStartTime.QuadPart) > 100, "Watchdog timer difference");
}

/**
 * @name TestEntry
 *
 * Test entry point.
 * This is called by DriverEntry as early as possible, but with ResultBuffer
 * initialized, so that test macros work correctly
 *
 * @param DriverObject
 *        Driver Object.
 *        This is guaranteed not to have been touched by DriverEntry before
 *        the call to TestEntry
 * @param RegistryPath
 *        Driver Registry Path
 *        This is guaranteed not to have been touched by DriverEntry before
 *        the call to TestEntry
 * @param DeviceName
 *        Pointer to receive a test-specific name for the device to create
 * @param Flags
 *        Pointer to a flags variable instructing DriverEntry how to proceed.
 *        See the KMT_TESTENTRY_FLAGS enumeration for possible values
 *        Initialized to zero on entry
 *
 * @return Status.
 *         DriverEntry will fail if this is a failure status
 */
NTSTATUS
TestEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PCUNICODE_STRING RegistryPath,
    OUT PCWSTR *DeviceName,
    IN OUT INT *Flags)
{
    PWATCHDOG pWd;
    LARGE_INTEGER sTick;
    struct DpcCtx sCtx;
    LARGE_INTEGER sDue;
    KDPC pDpc;

    PAGED_CODE();

    UNREFERENCED_PARAMETER(RegistryPath);
    UNREFERENCED_PARAMETER(Flags);

    pWd = WdAllocateWatchdog((PDEVICE_OBJECT)DriverObject, 
        WATCHDOG_MONITOR_KERNEL_TIME,
        (ULONG)'stmK' );
    
    ok_bool_true(pWd != NULL, "Watchdog normal allocation");
    ok_eq_pointer(WdGetDeviceObject(pWd), (PDEVICE_OBJECT)DriverObject);
    ok_eq_ulong(WdGetLastEvent(pWd), WATCHDOG_EVENT_NONE);

    sDue.HighPart = -1;
    sDue.LowPart = 100;
    sCtx.bOk = FALSE;

    KeInitializeDpc(&pDpc, (PKDEFERRED_ROUTINE)&KdeferredRoutine, &sTick);
    KeQueryTickCount(&sCtx.sStartTime);
    WdStartWatch(pWd, sDue, &pDpc);

    while (1)
    {
        KeQueryTickCount(&sTick);

        if (sTick.QuadPart >= (sCtx.sStartTime.QuadPart + 200))
            break;
    }
    WdSuspendWatch(pWd);

    ok_bool_true(sCtx.bOk, "Normal wd was called");

    sCtx.bOk = FALSE;

    WdFreeWatchdog(pWd);

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
    //WdFreeDeferredWatchdog(pWd); // TODO: this makes the driver crash, it calls KfFreeSpinLock

    return STATUS_SUCCESS;
}

VOID
TestUnload(
    IN PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();
}
