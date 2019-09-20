#ifndef __DETOURS_X_H__
#define __DETOURS_X_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define BYTE uint8_t
#define PBYTE uint8_t*
#define CHAR char
#define BOOLEAN uint8_t
#define SHORT short
#define USHORT unsigned short
#define PUSHORT unsigned short*
#define INT int
#define UINT unsigned int
#define INT32 int32_t
#define UINT16 uint16_t
#define BOOL int
#define LONG long
#define ULONG unsigned long
#define PULONG unsigned long*
#define VOID  void
#define PVOID void *
#define LONG64 uint64_t
#define UINT64 uint64_t
#define LONGLONG int64_t
#define ULONG_PTR uintptr_t
#define LONG_PTR  intptr_t
#define DWORD unsigned long
#define PDWORD DWORD *
#define WORD unsigned short
#define CONST const
#define HMODULE void*
#define HANDLE void*
#define SIZE_T size_t
#define LPCSTR const char*
#define LPVOID void*
#define LPCVOID const void*

#define FALSE 0
#define TRUE 1

#define _In_opt_
#define _Inout_opt_
#define _In_
#define _Out_opt_
#define _Benign_race_begin_
#define _Benign_race_end_

#define CALLBACK
#define WINAPI
#define UNALIGNED

#ifdef __i386__
#define _X86_
#endif

#ifdef __amd64__
#define _AMD64_
#endif

#ifdef __arm__
#define _ARM_
#endif

#ifdef __aarch64__
#define _ARM64_
#endif

#define ZeroMemory(x, y) bzero((x), (y))
#define CopyMemory(x, y, z) memcpy((x), (y), (z))
#define __debugbreak __builtin_trap

#define UNREFERENCED_PARAMETER(x) (void)(x)

#define C_ASSERT(x)

// Simulate Windows API

#define NO_ERROR 0
#define ERROR_INVALID_DATA      EINVAL
#define ERROR_INVALID_OPERATION ENOTSUP
#define ERROR_INVALID_PARAMETER EINVAL
#define ERROR_INVALID_HANDLE    EINVAL
#define ERROR_INVALID_BLOCK     EINVAL
#define ERROR_NOT_ENOUGH_MEMORY ENOMEM
#define SetLastError(x) errno = (x)
#define GetLastError(x) errno = (errno)

#define PtrToUlong(x) ((unsigned long)(x))

#ifdef SYS_gettid
#define GetCurrentThreadId() ((HANDLE)(uintptr_t)syscall(SYS_gettid))
#else
#error "SYS_gettid unavailable on this system"
#endif

#define GetCurrentProcess() ((HANDLE)(uintptr_t)getpid())

#define PAGE_EXECUTE_READ       (PROT_EXEC|PROT_READ)
#define PAGE_EXECUTE_READWRITE  (PROT_EXEC|PROT_READ|PROT_WRITE)
#define PAGE_EXECUTE_WRITECOPY  PAGE_EXECUTE_READWRITE
#define PAGE_EXECUTE            PROT_EXEC
#define PAGE_NOACCESS           PROT_NONE
#define PAGE_READONLY           PROT_READ
#define PAGE_READWRITE          (PROT_READ|PROT_WRITE)
#define PAGE_WRITECOPY          PAGE_READWRITE
BOOL VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, PDWORD lpflOldProtect);

#define MEM_RELEASE 0
BOOL VirtualFree( LPVOID lpAddress, SIZE_T dwSize, DWORD  dwFreeType);

BOOL FlushInstructionCache(HANDLE  hProcess, LPCVOID lpBaseAddress, SIZE_T  dwSize);

LONG InterlockedCompareExchange(LONG *Destination, LONG ExChange, LONG Comperand);

/*
    Internal functions
*/

bool detour_is_imported(PBYTE pbCode, PBYTE pbAddress);
PVOID detour_alloc_trampoline_allocate_new(PBYTE pbTarget, PVOID pLo, PVOID pHi);

#endif
