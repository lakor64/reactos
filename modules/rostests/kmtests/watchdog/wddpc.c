/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         DPC routine setup and helper
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wddpc.h"

void TestDpcRoutine(
  struct _KDPC  *Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2
)
{
    PDPCCTX ctx = (PDPCCTX)DeferredContext;

    // check if the millis timer is ok
    LARGE_INTEGER sCur;
    KeQueryTickCount(&sCur);
    ok_bool_true((sCur.QuadPart - ctx->sStartTime.QuadPart) > WATCHDOG_DEFAULT_TIME_DIFF, "Watchdog timer difference");

    KeSetEvent(&ctx->pkEvent, 0, FALSE);

    trace("DPC called!!!");
}

PDPCCTX TestSetWdContextAndDpc(void)
{
  PDPCCTX ctx = (PDPCCTX)ExAllocatePoolWithTag(NonPagedPool, sizeof(DPCCTX), WATCHDOG_TEST_TAG);
  if (!ctx)
    return NULL;

  KeQuerySystemTime(&ctx->sDueTime);
  ctx->sDueTime.LowPart += WATCHDOG_DEFAULT_TIME_DIFF*1000;

  KeInitializeEvent(&ctx->pkEvent, NotificationEvent, FALSE);
  KeInitializeDpc(&ctx->dpc, (PKDEFERRED_ROUTINE)&TestDpcRoutine, ctx);

  KeQueryTickCount(&ctx->sStartTime);

  return ctx;
}
