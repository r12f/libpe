#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEParserFactoryT
{
public:
    static PEParserT<T> * CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForMappedFile(void *pMemory, PEFileT<T> *pFile);
#ifdef LIBPE_WINOS
    static PEParserT<T> * CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile);
#endif
};

LIBPE_NAMESPACE_END