#include "ExecutableNative.hpp"
#include "ProcMaps.hpp"
#include "DetoursNix.hpp"

#include <dlfcn.h>
#include <assert.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace scimitar;

void WannaCry() {
    cout << "Wanna cry!\n";
}

void (*Say)() = WannaCry;

void Hahaha() {
    Say();
    cout << "Hahaha!\n";
}

int main(int argc, const char *argv[]) {
  
    DetourNixTransactionBegin();
    assert(DetourNixAttach((void**)&Say, (void*)&Hahaha) == 0);
    assert(DetourNixTransactionCommit() == 0);

    WannaCry();
}
