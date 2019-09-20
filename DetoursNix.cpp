#define DETOURS_X
#include "detours.h"

#include "ExecutableNative.hpp"
#include "ProcMaps.hpp"

#include <vector>

using namespace std;
using namespace scimitar;

ULONG WINAPI DetourPlatformGetModuleSize(_In_opt_ HMODULE hModule) {
    ExecutableNative EN(hModule);
    return EN.GetImageSize();
}

HMODULE WINAPI DetourPlatformGetContainingModule(_In_ PVOID pvAddr) {
    vector<ProcMaps> Entries;
    ProcMaps::Parse(ProcMaps::ReadSelf(), Entries, pvAddr);
    
    if (Entries.size() == 0)
        return NULL;

    return reinterpret_cast<void*>(Entries[0].Begin);
}

PVOID AllocateTrampoline(DWORD Size, uintptr_t Begin, uintptr_t End) {
    vector<ProcMaps> Entries;
    ProcMaps::Parse(ProcMaps::ReadSelf(), Entries, Begin, End);
    
    if (Entries.size() == 0)
        return NULL;

    void *pTry = NULL;
    auto NearRegion = Entries.begin();

    while (pTry == NULL || Begin < End) {
        Begin = ROUND_UP(Begin, 0x1000);
        
        if (Begin < NearRegion->Begin) {
            if ((NearRegion->Begin - Begin) >= Size) {
                pTry = mmap(reinterpret_cast<void*>(Begin), Size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_ANON|MAP_PRIVATE, 0, 0);
                if (pTry)
                    return pTry;
            } else {
                Begin = NearRegion->End;
                ++NearRegion;
                continue;
            }

            Begin += Size;
        } else {
            Begin = NearRegion->End;
            ++NearRegion;
        }
    }

    return pTry;
}

PVOID DetourPlatformAllocateTrampoline(DWORD Size, PBYTE pbTarget, PVOID pLo, PVOID pHi) {
    uintptr_t Begin = reinterpret_cast<uintptr_t>(pLo);
    uintptr_t End = reinterpret_cast<uintptr_t>(pHi);
    PVOID pTry = AllocateTrampoline(Size, reinterpret_cast<uintptr_t>(pbTarget), End);
    if (pTry == nullptr)
        pTry = AllocateTrampoline(Size, Begin, reinterpret_cast<uintptr_t>(pbTarget));
    return pTry;
}

BOOL DetourPlatformMemoryProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, PDWORD lpflOldProtect) {
    vector<ProcMaps> Entries;
    ProcMaps::Parse(ProcMaps::ReadSelf(), Entries, lpAddress);

    if (Entries.size() != 1)
        return FALSE;

    *lpflOldProtect = Entries[0].Prot;

    uintptr_t Address = ROUND_DOWN(reinterpret_cast<uintptr_t>(lpAddress), 0x1000);
    uintptr_t Size = dwSize + reinterpret_cast<uintptr_t>(lpAddress) - Address;

    return mprotect(reinterpret_cast<void*>(Address), Size, flNewProtect) == 0;
}

BOOL DetourPlatformMemoryFree( LPVOID lpAddress, SIZE_T dwSize, DWORD  dwFreeType) {
    UNREFERENCED_PARAMETER(dwFreeType);
    return munmap(lpAddress, dwSize) == 0;
}

/*
    DetoursNix
*/

long DetourNixTransactionBegin() {
    return DetourTransactionBegin();
}

long DetourNixTransactionAbort() {
    return DetourTransactionAbort();
}

long DetourNixTransactionCommit() {
    return DetourTransactionCommit();
}

long DetourNixTransactionCommitEx(void* **pppFailedPointer) {
    return DetourTransactionCommitEx(pppFailedPointer);
}

long DetourNixAttach(void* *ppPointer, void* pDetour) {
    return DetourAttach(ppPointer, pDetour);
}

long DetourNixAttachEx(void* *ppPointer, void* pDetour, void* *ppRealTrampoline, void* *ppRealTarget, void* *ppRealDetour) {
    return DetourAttachEx(ppPointer, pDetour, (PDETOUR_TRAMPOLINE *)ppRealTrampoline, ppRealTarget, ppRealDetour);
}

long DetourNixDetach(void* *ppPointer, void* pDetour) {
    return DetourDetach(ppPointer, pDetour);
}

void* DetourNixCopyInstruction(void* pDst, void* *ppDstPool, void* pSrc, void* *ppTarget, long *plExtra) {
    return DetourCopyInstruction(pDst, ppDstPool, pSrc, ppTarget, plExtra);
}

bool DetourNixSetCodeModule(void* hModule, bool fLimitReferencesToModule) {
    return DetourSetCodeModule(hModule, fLimitReferencesToModule);
}
