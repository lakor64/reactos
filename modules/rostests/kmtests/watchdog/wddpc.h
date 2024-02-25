/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         DPC routine setup and helper
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#pragma once

#define WATCHDOG_TEST_TAG ((ULONG)('dWeK'))
#define WATCHDOG_DEFAULT_TIME_DIFF (100*10000)

typedef struct DpcCtx
{
    LARGE_INTEGER sStartTime;
    KEVENT pkEvent;
    KDPC dpc;
    LARGE_INTEGER sDueTime;
} DPCCTX, *PDPCCTX;

extern void TestDpcRoutine(
  struct _KDPC  *Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2
);

extern PDPCCTX TestSetWdContextAndDpc(void);
