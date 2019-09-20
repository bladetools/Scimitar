#ifndef __EXECUTABLEELF_HPP__
#define __EXECUTABLEELF_HPP__

#include "Executable.hpp"

#include <elf.h>

#include <iostream>
#include <stdexcept>

#define FAST_FINDSYMBOL 0
#if FAST_FINDSYMBOL
#warning "GNU_HASH unimplemented"
#endif

namespace scimitar
{

struct ElfTypes32 {
    typedef Elf32_Half Elf_Half;
    typedef Elf32_Word Elf_Word;
    typedef Elf32_Sword Elf_Sword;
    typedef Elf32_Xword Elf_Xword;
    typedef Elf32_Sxword Elf_Sxword;
    typedef Elf32_Addr Elf_Addr;
    typedef Elf32_Off Elf_Off;
    typedef Elf32_Section Elf_Section;
    typedef Elf32_Half Elf_Versym;

    typedef Elf32_Ehdr Elf_Ehdr;
    typedef Elf32_Phdr Elf_Phdr;
    typedef Elf32_Shdr Elf_Shdr;
    typedef Elf32_Chdr Elf_Chdr;
    typedef Elf32_Sym  Elf_Sym;
    typedef Elf32_Syminfo Elf_Syminfo;
    typedef Elf32_Rel  Elf_Rel;
    typedef Elf32_Rela Elf_Rela;
    typedef Elf32_Dyn  Elf_Dyn;
    typedef Elf32_Nhdr Elf_Nhdr;
    typedef Elf32_Lib Elf_Lib;
};

struct ElfTypes64 {
    typedef Elf64_Half Elf_Half;
    typedef Elf64_Word Elf_Word;
    typedef Elf64_Sword Elf_Sword;
    typedef Elf64_Xword Elf_Xword;
    typedef Elf64_Sxword Elf_Sxword;
    typedef Elf64_Addr Elf_Addr;
    typedef Elf64_Off Elf_Off;
    typedef Elf64_Section Elf_Section;
    typedef Elf64_Half Elf_Versym;

    typedef Elf64_Ehdr Elf_Ehdr;
    typedef Elf64_Shdr Elf_Shdr;
    typedef Elf64_Phdr Elf_Phdr;
    typedef Elf64_Chdr Elf_Chdr;
    typedef Elf64_Sym  Elf_Sym;
    typedef Elf64_Syminfo Elf_Syminfo;
    typedef Elf64_Rel  Elf_Rel;
    typedef Elf64_Rela Elf_Rela;
    typedef Elf64_Dyn  Elf_Dyn;
    typedef Elf64_Nhdr Elf_Nhdr;
    typedef Elf64_Lib Elf_Lib;
};

#ifdef __LP64__

typedef ElfTypes64 ElfTypesNative;
#ifndef ELF_ST_TYPE
#define ELF_ST_BIND ELF64_ST_BIND
#define ELF_ST_TYPE ELF64_ST_TYPE
#endif

#ifndef ELF_ST_VISIBILITY
#ifndef ELF64_ST_VISIBILITY
#define ELF_ST_VISIBILITY(o) ((o)&0x3)
#else
#define ELF_ST_VISIBILITY ELF64_ST_VISIBILITY
#endif
#endif

#elif __ILP32__

typedef ElfTypes32 ElfTypesNative;
#ifndef ELF_ST_TYPE
#define ELF_ST_BIND ELF32_ST_BIND
#define ELF_ST_TYPE ELF32_ST_TYPE
#endif

#ifndef ELF_ST_VISIBILITY
#ifndef ELF32_ST_VISIBILITY
#define ELF_ST_VISIBILITY(o) ((o)&0x3)
#else
#define ELF_ST_VISIBILITY ELF32_ST_VISIBILITY
#endif
#endif

#else
#error "Unknown Architicture"
#endif

template <class ElfTypes> class ExecutableELF;

template <class ElfTypes>
class SymbolIteratorELF : public std::iterator<std::forward_iterator_tag, Symbol>
{
private:
    ExecutableELF<ElfTypes> &Executable;
    int Index;
public:
    SymbolIteratorELF(ExecutableELF<ElfTypes> &E, int I) : Executable(E), Index(I) {

    }

    SymbolIteratorELF& operator =(const SymbolIteratorELF &SI) {
        Executable = SI.Executable;
        Index = SI.Index;
        return *this;
    }

    Symbol operator*() {
        if (Index >= Executable.size())
            throw std::logic_error("Fetch symbol from end iterator");

        typename ElfTypes::Elf_Sym *Sym = &Executable.SymbolTable[Index];

        return Executable.ConvertELFSymbol(Sym);
    }

    SymbolIteratorELF& operator ++() {
        if (Index >= Executable.size())
            throw std::logic_error("Increment an end iterator");
        
        Index ++;
        return *this;
    }

    SymbolIteratorELF operator ++(int) {
        if (Index >= Executable.size())
            throw std::logic_error("Increment an end iterator");
        
        auto SI = *this;

        Index ++;
        return SI;
    }

    bool operator==(const SymbolIteratorELF &SI) {
        return Index == SI.Index;
    }

    bool operator!=(const SymbolIteratorELF &SI) {
        return !(*this == SI);
    }
};

template <class ElfTypes>
class ExecutableELF : public Executable
{
public:
    typedef typename ElfTypes::Elf_Half Elf_Half;
    typedef typename ElfTypes::Elf_Word Elf_Word;
    typedef typename ElfTypes::Elf_Sword Elf_Sword;
    typedef typename ElfTypes::Elf_Xword Elf_Xword;
    typedef typename ElfTypes::Elf_Sxword Elf_Sxword;
    typedef typename ElfTypes::Elf_Addr Elf_Addr;
    typedef typename ElfTypes::Elf_Off Elf_Off;
    typedef typename ElfTypes::Elf_Section Elf_Section;
    typedef typename ElfTypes::Elf_Versym Elf_Versym;

    typedef typename ElfTypes::Elf_Ehdr Elf_Ehdr;
    typedef typename ElfTypes::Elf_Shdr Elf_Shdr;
    typedef typename ElfTypes::Elf_Chdr Elf_Chdr;
    typedef typename ElfTypes::Elf_Sym Elf_Sym;
    typedef typename ElfTypes::Elf_Syminfo Elf_Syminfo;
    typedef typename ElfTypes::Elf_Rel Elf_Rel;
    typedef typename ElfTypes::Elf_Rela Elf_Rela;
    typedef typename ElfTypes::Elf_Phdr Elf_Phdr;
    typedef typename ElfTypes::Elf_Dyn Elf_Dyn;
    typedef typename ElfTypes::Elf_Nhdr Elf_Nhdr;
    typedef typename ElfTypes::Elf_Lib Elf_Lib;

    explicit ExecutableELF(void *Header) : 
        Executable(NULL, NULL),
        StringTable(nullptr),
        SymbolTable(nullptr),
#if FAST_FINDSYMBOL
        NumBucket(0),
        Bucket(nullptr),
        Chain(nullptr),
#endif
        NumSymbol(0)
    {
        Elf_Ehdr *Ehdr = reinterpret_cast<Elf_Ehdr*>(Header);

        if (memcmp(Ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
            throw std::invalid_argument("Invalid ELF magic");
        }

        Elf_Phdr *Phdr = reinterpret_cast<Elf_Phdr*>(reinterpret_cast<uintptr_t>(Header) + Ehdr->e_phoff);
        Elf_Phdr *PhdrLoadMin = nullptr;
        Elf_Phdr *PhdrLoadMax = nullptr;
        Elf_Phdr *PhdrDynamic = nullptr;

        for (int i = 0 ; i < Ehdr->e_phnum; ++i, ++Phdr) {
            if (PhdrLoadMax == nullptr || (Phdr->p_type == PT_LOAD && Phdr->p_vaddr > PhdrLoadMax->p_vaddr)) {
                PhdrLoadMax = Phdr;
            }
            if (PhdrLoadMin == nullptr || (Phdr->p_type == PT_LOAD && Phdr->p_vaddr < PhdrLoadMin->p_vaddr)) {
                PhdrLoadMin = Phdr;
            }
            if (Phdr->p_type == PT_DYNAMIC)
                PhdrDynamic = Phdr;
        }

        if (PhdrLoadMax == nullptr || PhdrLoadMin == nullptr)
            throw std::invalid_argument("Invalid executable image");

        if (PhdrDynamic == nullptr)
            throw std::invalid_argument("Missing DT_DYNAMIC"); 

        Elf_Off ImageSize = ROUND_UP((PhdrLoadMax->p_vaddr + PhdrLoadMax->p_memsz) - PhdrLoadMin->p_vaddr, 0x1000);

        ImageBegin = Header;
        ImageEnd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ImageBegin) + ImageSize);
        ImageBias = Bias(reinterpret_cast<uintptr_t>(ImageBegin), PhdrLoadMin->p_vaddr);

        // Dynamic is already relocated in glibc. But android not.
        Bias Base(0, 0);
    #ifdef __ANDROID__
        Base = ImageBias;
    #endif
        
        Elf_Dyn *Dynamic = reinterpret_cast<Elf_Dyn*>(ImageBias(PhdrDynamic->p_vaddr));
        for (Elf_Dyn *D = Dynamic; D->d_tag != DT_NULL; ++ D) {
            switch (D->d_tag) {
                case DT_HASH:
                    NumSymbol = reinterpret_cast<uint32_t*>(Base(D->d_un.d_ptr))[1];
#if FAST_FINDSYMBOL
                    NumBucket = reinterpret_cast<uint32_t*>(Base(D->d_un.d_ptr))[0];
                    Bucket = reinterpret_cast<uint32_t*>(Base(D->d_un.d_ptr) + 8);
                    Chain = reinterpret_cast<uint32_t*>(Base(D->d_un.d_ptr) + 8 + NumBucket * 4);
#endif
                    break;
                case DT_GNU_HASH:
                    NumSymbol = GetNumberOfSymbolsFromGnuHash(reinterpret_cast<uint32_t*>(Base(D->d_un.d_ptr)));
                    break;
                case DT_SYMTAB:
                    SymbolTable = reinterpret_cast<Elf_Sym*>(Base(D->d_un.d_ptr));
                    break;
                case DT_STRTAB:
                    StringTable = reinterpret_cast<const char*>(Base(D->d_un.d_ptr));
                    break;
            }
        }

        if (SymbolTable == nullptr || StringTable == nullptr)
            throw std::invalid_argument("Missing symbol information");

#if FAST_FINDSYMBOL
        if (Bucket == nullptr || Chain == nullptr)
            throw std::invalid_argument("Missing hash information");
#endif
    };

    virtual ~ExecutableELF() {

    }

    void * GetImageEntryPoint() override {
        return reinterpret_cast<void*>(ImageBias(reinterpret_cast<Elf_Ehdr*>(ImageBegin)->e_entry));
    }

    SymbolIteratorELF<ElfTypes> begin() {
        return SymbolIteratorELF<ElfTypes>(*this, 0);
    }

    SymbolIteratorELF<ElfTypes> end() {
        return SymbolIteratorELF<ElfTypes>(*this, NumSymbol);
    }

    size_t size() {
        return NumSymbol;
    }

    Symbol ConvertELFSymbol(Elf_Sym *Sym) {
        Symbol S;

        S.Name = StringTable + Sym->st_name;
        S.Address = reinterpret_cast<void*>(ImageBias(Sym->st_value));

        switch (ELF_ST_BIND(Sym->st_info)) {
            case STB_GLOBAL:S.Bind = Symbol::GLOBAL;break;
            case STB_WEAK:S.Bind = Symbol::WEAK;break;
            case STB_LOCAL:S.Bind = Symbol::LOCAL;break;
        }

        switch (ELF_ST_TYPE(Sym->st_info)) {
            case STT_NOTYPE:S.Type = Symbol::NOTYPE;break;
            case STT_OBJECT:S.Type = Symbol::OBJECT;break;
            case STT_FUNC:S.Type = Symbol::FUNCTION;break;
            case STT_SECTION:S.Type = Symbol::SECTION;break;
            case STT_FILE:S.Type = Symbol::FILE;break;
            case STT_COMMON:S.Type = Symbol::COMMON;break;
            case STT_TLS:S.Type = Symbol::TLS;break;
        }

        switch (ELF_ST_VISIBILITY(Sym->st_other)) {
            case STV_DEFAULT: S.Visibility = Symbol::DEFAULT; break;
            case STV_INTERNAL: S.Visibility = Symbol::INTERNAL; break;
            case STV_HIDDEN: S.Visibility = Symbol::HIDDEN; break;
            case STV_PROTECTED: S.Visibility = Symbol::PROTECTED; break;
        }

        return S;
    }
#if FAST_FINDSYMBOL
    static
    uint32_t SymbolHash(const char *SymbolName) {
        uint32_t h = 0, g;
        while(*SymbolName) {
            h = (h << 4) + *SymbolName ++;
            g = h & 0xf0000000;
            h ^= g;
            h ^= g >> 24;
        }
        return h;
    }
#endif

    void *FindSymbol(const char *SymbolName) override {
#if FAST_FINDSYMBOL
        uint32_t Hash = SymbolHash(SymbolName);
        size_t N = Bucket[Hash % NumBucket];
        for (; N != 0; N = Chain[N])
        {
            Elf_Sym *Sym = &SymbolTable[N];
            if (strcmp(Sym->st_name + StringTable, SymbolName))
                continue;
            switch (ELF_ST_BIND(Sym->st_info)) {
                case STB_GLOBAL:
                case STB_WEAK:
                    if (Sym->st_shndx == SHN_UNDEF)
                        continue;
                    return reinterpret_cast<void*>(ImageBias(Sym->st_value));
            }
        }
#else
        for (auto S : *this) {
            if (S.Name == SymbolName)
                return S.Address;
        }
#endif
        return NULL;
    }

protected:
    const char *StringTable;
    Elf_Sym *SymbolTable;
    
#if FAST_FINDSYMBOL
    uint32_t NumBucket;
    uint32_t *Bucket;
    uint32_t *Chain;
#endif
    uint32_t NumSymbol;

    friend class SymbolIteratorELF<ElfTypes>;

private:
    uint32_t GetNumberOfSymbolsFromGnuHash(uint32_t *GnuHash) {
        uint32_t GnuNumBucket = GnuHash[0];
        uint32_t GnuSymbolOffset = GnuHash[1];

        uint32_t GnuBloomSize = GnuHash[2];
        uint32_t GnuBloomShift = GnuHash[3];
        
        uintptr_t *GnuBloom = reinterpret_cast<uintptr_t*>(&GnuHash[4]);
        uint32_t *GnuBucket = reinterpret_cast<uint32_t*>(&GnuBloom[GnuBloomSize]);
        uint32_t *GnuChain = reinterpret_cast<uint32_t*>(&GnuBucket[GnuNumBucket]);
        
        int N = 0;
        for (size_t i = 0; i < GnuNumBucket; i++)
        {
            if (GnuBucket[i] > N)
                N = GnuBucket[i];
        }
        
        while ((GnuChain[N - GnuSymbolOffset] & 1) == 0)
            N ++;
        
        return N + 1;
    }
};



} // namespace scimitar

#endif
