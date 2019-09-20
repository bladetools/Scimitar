#include "ExecutableNative.hpp"
#include "ProcMaps.hpp"

#include <dlfcn.h>
#include <assert.h>

#include <iostream>
#include <vector>

#ifdef __APPLE__
#error "Unimplemented"
#else
#define MATCH_REGEX ".*libc[-.].*so.*$"
#endif

using namespace std;
using namespace scimitar;

int main(int argc, const char *argv[]) {
    vector<ProcMaps> Entries;
    // ProcMaps::Parse(ProcMaps::ReadSelf(), Entries, ".*ExecutableTest$", false);
    ProcMaps::Parse(ProcMaps::ReadSelf(), Entries, MATCH_REGEX, false);

    assert(Entries.size() != 0 && "libc not found");

    cout << ProcMaps::ReadSelf() << endl;

    void * Header = reinterpret_cast<void*>(Entries[0].Begin);
    ExecutableNative Exe(Header);

    cout << "File: " << Entries[0].PathName << endl;
    cout << "Start:" << Exe.ImageBegin << endl;
    cout << "Size: " << Exe.GetImageSize() << endl;
    cout << "Symbol Count: " << Exe.size() << endl;
    cout << "EntryPoint: " << Exe.GetImageEntryPoint() << endl;

    cout << "Symbols:" << endl;
    cout << "FindSymbol(\"system\") => " << hex << Exe.FindSymbol("system") << endl;;
    cout << "dlsym(nullptr, \"system\") => " << hex << dlsym(nullptr, "system") << endl;;
    
    for (auto S : Exe) {
        cout << S.Address << "  " << S.Name << endl;
    }
}
