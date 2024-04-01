/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         DPC routine setup and helper
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wddpc.h"
#include <watchdog.h>

void NTAPI TestDpcRoutine(
  struct _KDPC  *Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2
)
{
    PDPCCTX ctx = (PDPCCTX)DeferredContext;

    // check if the millis timer is ok
    LARGE_INTEGER sCur;
    KeQuerySystemTime(&sCur);
    ok_bool_true((sCur.QuadPart - ctx->sStartTime.QuadPart) >= WATCHDOG_NORMAL_MINIMUM_100NS_DUE_TIME, "Watchdog timer difference");

    KeSetEvent(&ctx->pkEvent, 0, FALSE);

    trace("DPC called!!! TIME: %llu START_TIME: %llu\n", sCur.QuadPart, ctx->sStartTime.QuadPart);
}

PDPCCTX TestSetWdContextAndDpc(void)
{
  PDPCCTX ctx = (PDPCCTX)ExAllocatePoolWithTag(NonPagedPool, sizeof(DPCCTX), WATCHDOG_TEST_TAG);
  if (!ctx)
    return NULL;

  KeQuerySystemTime(&ctx->sDueTime);
  ctx->sDueTime.QuadPart += WATCHDOG_100NS_TIME;

  KeInitializeEvent(&ctx->pkEvent, NotificationEvent, FALSE);
  KeInitializeDpc(&ctx->dpc, &TestDpcRoutine, ctx);

  KeQueryTickCount(&ctx->sStartTime);

  return ctx;
}
