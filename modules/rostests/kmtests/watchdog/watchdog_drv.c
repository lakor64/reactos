/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         Kernel-Mode Test Suite for watchdog.sys
 * PROGRAMMER:      Christian Rendina <pizzaiolo100@proton.me>
 */

#include <kmt_test.h>
#include "wdioctl.h"
#include "wddpc.h"

extern KMT_MESSAGE_HANDLER TestNormalWd;
extern KMT_MESSAGE_HANDLER TestDeferredWd;

static struct
{
    ULONG ControlCode;
    PKMT_MESSAGE_HANDLER Handler;
} MessageHandlers[] =
{
    { IOCTL_TEST_WATCHDOG_NORMAL,          TestNormalWd },
    { IOCTL_TEST_WATCHDOG_DEFERRED,        TestDeferredWd },
};

NTSTATUS
TestEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PCUNICODE_STRING RegistryPath,
    _Out_ PCWSTR *DeviceName,
    _Inout_ INT *Flags)
{
    ULONG i;
    
    PAGED_CODE();

    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    UNREFERENCED_PARAMETER(Flags);

    *DeviceName = L"Watchdog";

    for (i = 0; i < (sizeof(MessageHandlers) / sizeof(MessageHandlers[0])); i++)
        KmtRegisterMessageHandler(MessageHandlers[i].ControlCode, NULL, MessageHandlers[i].Handler);

    trace("Loaded Watchdog driver!!!\n");

    return STATUS_SUCCESS;
}

VOID
TestUnload(
    _In_ PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();

    UNREFERENCED_PARAMETER(DriverObject);
}
