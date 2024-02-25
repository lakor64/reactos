/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         User mod Test Suite for watchdog.sys
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wdioctl.h"

static
DWORD
LoadWdDriver(void)
{
    DWORD Error;

    /* Start the special-purpose driver */
    Error = KmtLoadAndOpenDriver(L"Watchdog", FALSE);
    ok_eq_int(Error, ERROR_SUCCESS);
    if (Error)
        return Error;

    return ERROR_SUCCESS;
}

static
void
UnloadWdDriver(void)
{
    /* Stop the driver. */
    KmtCloseDriver();
    KmtUnloadDriver();
}

START_TEST(WatchdogNormal)
{
    DWORD Error;

    Error = LoadWdDriver();
    ok_eq_int(Error, 0);
    if (Error)
        return;

    Error = KmtSendToDriver(IOCTL_TEST_WATCHDOG_NORMAL);
    ok_eq_ulong(Error, ERROR_SUCCESS);

    UnloadWdDriver();
}

START_TEST(WatchdogDeferred)
{
    DWORD Error;

    Error = LoadWdDriver();
    ok_eq_int(Error, 0);
    if (Error)
        return;

    Error = KmtSendToDriver(IOCTL_TEST_WATCHDOG_DEFERRED);
    ok_eq_ulong(Error, ERROR_SUCCESS);

    UnloadWdDriver();
}
