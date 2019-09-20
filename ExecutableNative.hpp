#ifndef __EXECUTABLENATIVE_H__
#define __EXECUTABLENATIVE_H__

#ifdef __APPLE__
    #error "Unimplemented"
#else 
    #include "ExecutableELF.hpp"
    namespace scimitar {
        typedef ExecutableELF<ElfTypesNative> ExecutableNative;
        typedef SymbolIteratorELF<ElfTypesNative> SymbolIteratorNative;
    }
#endif

#endif
