#include "detours.h"

/*
    Platform interfaces
*/

ULONG DetourPlatformGetModuleSize(_In_opt_ HMODULE hModule);
HMODULE DetourPlatformGetContainingModule(_In_ PVOID pvAddr);
PVOID DetourPlatformAllocateTrampoline(DWORD Size, PBYTE pbTarget, PVOID pLo, PVOID pHi); 
BOOL DetourPlatformMemoryProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, PDWORD lpflOldProtect);
BOOL DetourPlatformMemoryFree( LPVOID lpAddress, SIZE_T dwSize, DWORD  dwFreeType);

/*
    Detours base on platform interface
*/

ULONG WINAPI DetourGetModuleSize(_In_opt_ HMODULE hModule) {
    return DetourPlatformGetModuleSize(hModule);
}

HMODULE WINAPI DetourGetContainingModule(_In_ PVOID pvAddr) {
    return DetourPlatformGetContainingModule(pvAddr);
}

bool detour_is_imported(PBYTE pbCode, PBYTE pbAddress) {
    HMODULE hModule = DetourPlatformGetContainingModule(pbCode);
    ULONG ModuleSize = DetourPlatformGetModuleSize(hModule);
    return pbAddress >= (PBYTE)hModule && pbAddress < ((PBYTE)hModule + ModuleSize);
}

PVOID detour_alloc_trampoline_allocate_new(PBYTE pbTarget, PVOID pLo, PVOID pHi) {
    return DetourPlatformAllocateTrampoline(DETOUR_REGION_SIZE, pbTarget, pLo, pHi);
}

/*
    Window APIs
*/

BOOL VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, PDWORD lpflOldProtect) {
    return DetourPlatformMemoryProtect(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

BOOL VirtualFree( LPVOID lpAddress, SIZE_T dwSize, DWORD  dwFreeType) {
    return DetourPlatformMemoryFree(lpAddress, dwSize, dwFreeType);
}

BOOL FlushInstructionCache(HANDLE  hProcess, LPCVOID lpBaseAddress, SIZE_T  dwSize) {
    // TODO
    return TRUE;
}

LONG InterlockedCompareExchange(LONG *Destination, LONG ExChange, LONG Comperand) {
    // TODO
    LONG InitialValue = *Destination;
    if (InitialValue == Comperand)
        *Destination = ExChange;
    return InitialValue;
}
