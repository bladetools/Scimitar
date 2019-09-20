#include <sys/mman.h>

#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "ProcMaps.hpp"

namespace scimitar
{

using namespace std;

string PidToPath(const string &Pid)
{
    ostringstream oss;
    oss << "/proc/" << Pid << "/maps";
    return oss.str();
}

string ProcMaps::ToString()
{
    ostringstream oss;
    oss << hex << setfill('0') << setw(8) << Begin << '-' << End << ' ';
    oss << ((Prot & PROT_READ) ? 'r' : '-');
    oss << ((Prot & PROT_WRITE) ? 'w' : '-');
    oss << ((Prot & PROT_EXEC) ? 'x' : '-');
    oss << (Shared ? 's' : 'p') << ' ';
    oss << hex << setfill('0') << setw(8) << Offset << ' ' << dec;
    oss << Major << ':' << Minor << ' ' << Inode;
    oss << "    " << PathName << endl;
    return oss.str();
}

string ProcMaps::ReadSelf()
{
    return ReadFile(PidToPath("self"));
}

string ProcMaps::ReadPid(pid_t Pid)
{
    return ReadFile(PidToPath(to_string(Pid)));
}

string ProcMaps::ReadFile(const std::string &PathName)
{
    ostringstream oss;
    ifstream is(PathName);

    if (is.is_open())
    {
        oss << is.rdbuf();
    }

    return oss.str();
}

bool ProcMaps::Parse(const string &Maps, vector<ProcMaps> &Entries, std::function<bool(const ProcMaps &)> Filter)
{
    if (Maps.size() == 0)
        return false;

    std::regex EntryRegex("([0-9a-f]+)-([0-9a-f]+)\\s+([rwxps-]{4})\\s+([0-9a-f]+)\\s+(\\d+):(\\d+)\\s+(\\d+)[ \\t\\r]*(.*)\n",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);
    auto EntriesBegin = sregex_iterator(Maps.begin(), Maps.end(), EntryRegex);
    auto EntriesEnd = sregex_iterator();

    for (std::sregex_iterator i = EntriesBegin; i != EntriesEnd; ++i)
    {
        std::smatch match = *i;

        ProcMaps Entry;
        Entry.Begin = strtoul(match[1].str().c_str(), nullptr, 16);
        Entry.End = strtoul(match[2].str().c_str(), nullptr, 16);

        const string Perms = match[3].str();
        if (Perms[0] != '-')
            Entry.Prot |= PROT_READ;
        if (Perms[1] != '-')
            Entry.Prot |= PROT_WRITE;
        if (Perms[2] != '-')
            Entry.Prot |= PROT_EXEC;
        Entry.Shared = Perms[3] == 's';

        Entry.Offset = strtoul(match[4].str().c_str(), nullptr, 16);

        Entry.Major = strtoul(match[5].str().c_str(), nullptr, 10);
        Entry.Minor = strtoul(match[6].str().c_str(), nullptr, 10);

        Entry.Inode = strtoul(match[7].str().c_str(), nullptr, 10);

        Entry.PathName = match[8].str();

        if (Filter(Entry))
            Entries.push_back(std::move(Entry));
    }

    return true;
}

bool ProcMaps::Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, const std::string &Match, bool IgnoreCase) {
    regex::flag_type Flags = std::regex_constants::ECMAScript | std::regex_constants::extended;
    if (IgnoreCase)
        Flags |= std::regex_constants::icase;
    
    std::regex MatchRegex(Match, Flags);

    return Parse(Maps, Entries, [&](auto &PM) { return std::regex_match(PM.PathName, MatchRegex); });
}

} // namespace scimitar
