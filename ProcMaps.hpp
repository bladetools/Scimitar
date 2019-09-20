#ifndef __PROCMAPS_HPP__
#define __PROCMAPS_HPP__

#include <sys/types.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace scimitar
{

class ProcMaps
{
public:
    uintptr_t Begin;
    uintptr_t End;
    unsigned int Prot;
    bool Shared;
    uintptr_t Offset;
    unsigned int Major;
    unsigned int Minor;
    unsigned long Inode;
    std::string PathName;

    ProcMaps() : Begin(0),
                 End(0),
                 Prot(0),
                 Shared(0),
                 Offset(0),
                 Major(0),
                 Minor(0),
                 Inode(0),
                 PathName()
    {
    }

    ProcMaps(const ProcMaps &PM) : Begin(PM.Begin),
                                   End(PM.End),
                                   Prot(PM.Prot),
                                   Shared(PM.Shared),
                                   Offset(PM.Offset),
                                   Major(PM.Major),
                                   Minor(PM.Minor),
                                   Inode(PM.Inode),
                                   PathName(PM.PathName)
    {
    }

    ProcMaps(const ProcMaps &&PM) : Begin(PM.Begin),
                                    End(PM.End),
                                    Prot(PM.Prot),
                                    Shared(PM.Shared),
                                    Offset(PM.Offset),
                                    Major(PM.Major),
                                    Minor(PM.Minor),
                                    Inode(PM.Inode),
                                    PathName(std::move(PM.PathName))
    {
    }

    std::string ToString();

    static std::string ReadSelf();
    static std::string ReadPid(pid_t Pid);
    static std::string ReadFile(const std::string &PathName);

    static bool Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, std::function<bool(const ProcMaps &)> Filter);

    static bool Parse(const std::string &Maps, std::vector<ProcMaps> &Entries)
    {
        return Parse(Maps, Entries, [](auto &PM) { return true; });
    }

    static bool Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, const std::string &File)
    {
        return Parse(Maps, Entries, [&](auto &PM) { return PM.PathName.find(File) != std::string::npos; });
    }

    static bool Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, const std::string &Match, bool icase);

    template <class A>
    static
        typename std::enable_if<std::is_arithmetic<A>::value || std::is_pointer<A>::value, bool>::type
        Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, A Address)
    {
        uintptr_t Address_ = (uintptr_t)Address;
        return Parse(Maps, Entries, [&](auto &PM) { return PM.Begin <= Address_ && PM.End > Address_; });
    }

    template <class A>
    static
        typename std::enable_if<std::is_arithmetic<A>::value || std::is_pointer<A>::value, bool>::type
        Parse(const std::string &Maps, std::vector<ProcMaps> &Entries, A Begin, A End)
    {
        uintptr_t Begin_ = (uintptr_t)Begin;
        uintptr_t End_ = (uintptr_t)End;
        return Parse(Maps, Entries, [&](auto &PM) { return (PM.Begin >= Begin_ || PM.End > Begin_) && (PM.End < End_ || PM.Begin < End_); });
    }
};

} // namespace scimitar

#endif
