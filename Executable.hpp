#ifndef __EXECUTABLE_HPP__
#define __EXECUTABLE_HPP__

#include <string>

#define ROUND_DOWN(x, n) ((x) & ~((uintptr_t)(n) - 1))
#define ROUND_UP(x, n) (((x) + (n) - 1) & ~((uintptr_t)(n) - 1))

namespace scimitar
{

class Executable
{
public:
    struct Bias {
        uintptr_t Base;
        uintptr_t VAddr;

        Bias() : Base(0), VAddr(0) { }
        Bias(uintptr_t B, uintptr_t VA) : Base(B), VAddr(VA) { }

        template <class AT>
        typename std::enable_if<std::is_arithmetic<AT>::value || std::is_pointer<AT>::value, uintptr_t>::
        type operator ()(AT Address) {
            uintptr_t Address_ = (uintptr_t)Address;
            return Base + Address_ - VAddr;
        }
    };

    void *ImageBegin;
    void *ImageEnd;
    Bias ImageBias;

    Executable(void *Begin, void *End) : ImageBegin(Begin), ImageEnd(End) { }
    virtual ~Executable() { };

    template <class T>
    typename std::enable_if<!std::is_same<T, void*>::value, T>::type
    FindSymbol(const char *Symbol)
    {
        return reinterpret_cast<T>(FindSymbol(Symbol));
    };

    uintptr_t GetImageSize() {
        return (uintptr_t)ImageEnd - (uintptr_t)ImageBegin;
    }

    virtual void * GetImageEntryPoint() {
        return NULL;
    }

    virtual void *FindSymbol(const char *Symbol) = 0;
};

struct Symbol {
    enum SymbolBind {
        UNKNOWN_BIND,
        LOCAL,
        GLOBAL,
        WEAK
    };
    enum SymbolType {
        UNKNOWN_TYPE,
        NOTYPE,
        OBJECT,
        FUNCTION,
        SECTION,
        FILE,
        COMMON,
        TLS
    };
    enum SymbolVisibility {
        DEFAULT,
        INTERNAL,
        HIDDEN,
        PROTECTED
    };

    const char *Name;
    void *Address;
    SymbolBind Bind;
    SymbolType Type;
    SymbolVisibility Visibility;
    int SectionIndex;
};

} // namespace scimitar

#endif
