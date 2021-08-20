#include "pch.h"

static BOOL(WINAPI* TrueVerifyVersionInfoW)(
    _Inout_ LPOSVERSIONINFOEXW lpVersionInformation,
    _In_    DWORD dwTypeMask,
    _In_    DWORDLONG dwlConditionMask) = VerifyVersionInfoW;

BOOL
WINAPI
VerifyVersionInfoW_Win10(
    _Inout_ LPOSVERSIONINFOEXW lpVersionInformation,
    _In_    DWORD dwTypeMask,
    _In_    DWORDLONG dwlConditionMask
)
{
    if (lpVersionInformation->dwMajorVersion == 10)
        return TRUE;
    else
        return FALSE;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    LONG error;
    (void)hinst;
    (void)reserved;

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TrueVerifyVersionInfoW, VerifyVersionInfoW_Win10);
        error = DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueVerifyVersionInfoW, VerifyVersionInfoW_Win10);
        error = DetourTransactionCommit();
    }
    return TRUE;
}

//
///////////////////////////////////////////////////////////////// End of File.
