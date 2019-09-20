#include <iostream>
#include <vector>
#include <functional>

#include "ProcMaps.hpp"

using namespace std;
using namespace scimitar;

void test(const string &file, int N, function<bool(const ProcMaps &)> Filter) {
    vector<ProcMaps> Entries;
    ProcMaps::Parse(ProcMaps::ReadFile(file), Entries, Filter);

    cout << file << " ";

    cout << (Entries.size() == N ? "pass" : "failed") << '\n';
}

int main(int argc, const char *argv[]) {

    cout << "test number:\n";
    test("scimitar/tests/1.1.txt", 1, [](auto &PM){ return true; });
    test("scimitar/tests/1.txt", 1, [](auto &PM){ return true; });
    test("scimitar/tests/2.txt", 2, [](auto &PM){ return true; });
    test("scimitar/tests/5.txt", 5, [](auto &PM){ return true; });
    test("scimitar/tests/8.txt", 8, [](auto &PM){ return true; });

    cout << "test filter:\n"; 
    test("scimitar/tests/8.txt", 4, [](auto &PM){ return PM.PathName.find("/usr/lib64/libc-2.15.so") != string::npos; });
    test("scimitar/tests/8.txt", 1, [](auto &PM){ return PM.Begin <= 0x35b1dac000 && PM.End > 0x35b1dac000; });

    cout << "test regex filter: ";
    vector<ProcMaps> Entries;
    ProcMaps::Parse(ProcMaps::ReadFile("scimitar/tests/8.txt"), Entries, ".*\\.so(\\s*\\(.*\\)|.*)$", true);
    ProcMaps::Parse(ProcMaps::ReadFile("scimitar/tests/8.1.txt"), Entries, ".*\\.so(\\s*\\(.*\\)|.*)$", true);
    cout << ((Entries.size() == 14) ? "pass\n" : "failed\n");

    return 0;
}
