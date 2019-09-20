#ifndef __DETOURSNIX_HPP__
#define __DETOURSNIX_HPP__

#include <stdio.h>

long DetourNixTransactionBegin();
long DetourNixTransactionAbort();
long DetourNixTransactionCommit();
long DetourNixTransactionCommitEx(void* **pppFailedPointer);

long DetourNixAttach(void* *ppPointer,
                        void* pDetour);

long DetourNixAttachEx(void* *ppPointer,
                           void* pDetour,
                           void* *ppRealTrampoline,
                           void* *ppRealTarget,
                           void* *ppRealDetourNix);

long DetourNixDetach(void* *ppPointer,
                        void* pDetour);

void* DetourNixCopyInstruction(void* pDst,
                                void* *ppDstPool,
                                void* pSrc,
                                void* *ppTarget,
                                long *plExtra);

bool DetourNixSetCodeModule(void* hModule,
                                bool fLimitReferencesToModule);

#endif
