/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         DPC routine setup and helper
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#pragma once

#define WATCHDOG_TEST_TAG ((ULONG)('tseT'))
#define WATCHDOG_100NS_TIME 300000 // 30ms
#define WATCHDOG_100NS_TIME_CHECK 600000 // 60ms
#define WATCHDOG_NORMAL_MINIMUM_100NS_DUE_TIME_CHECK 0xC0100001

typedef struct DpcCtx
{
    LARGE_INTEGER sStartTime;
    KEVENT pkEvent;
    KDPC dpc;
    LARGE_INTEGER sDueTime;
} DPCCTX, *PDPCCTX;

extern void NTAPI TestDpcRoutine(
  struct _KDPC  *Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2
);

extern PDPCCTX TestSetWdContextAndDpc(void);
