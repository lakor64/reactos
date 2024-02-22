/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         User mode part of the TcpIp.sys test suite
 * PROGRAMMER:      Jérôme Gardou <jerome.gardou@reactos.org>
 */

#include <kmt_test.h>

static
DWORD
LoadDriver(void)
{
    DWORD Error;

    /* Start the special-purpose driver */
    Error = KmtLoadAndOpenDriver(L"watchdog", FALSE);
    ok_eq_int(Error, ERROR_SUCCESS);
    if (Error)
        return Error;

    return ERROR_SUCCESS;
}

static
void
UnloadDriver(void)
{
    /* Stop the driver. */
    KmtCloseDriver();
    KmtUnloadDriver();
}

START_TEST(Watchdog)
{
    DWORD Error;

    Error = LoadDriver();
    ok_eq_int(Error, 0);
    if (Error)
        return;

    UnloadDriver();
}
