#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEParserDiskFileT :
    public PEParserT<T>
{
public:
    PEParserDiskFileT() {}
    virtual ~PEParserDiskFileT() {}

    virtual PEParserType GetType() { return PE_PARSER_TYPE_DISK_FILE; }
    virtual bool_t IsRawAddressVA() { return false; }
    virtual error_t ParseBasicInfo();
    virtual error_t ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection);

protected:
    virtual LibPEAddressT(T) GetAddressFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetAddressFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) GetAddressFromFOA(LibPEAddressT(T) nFOA);
};
typedef PEParserDiskFileT<PE32> PEParserDiskFile32;
typedef PEParserDiskFileT<PE64> PEParserDiskFile64;

template <class T>
class PEParserMappedFileT :
    public PEParserT<T>
{
public:
};

#ifdef LIBPE_WINOS

template <class T>
class PEParserMappedResourceT :
    public PEParserT<T>
{
public:
};

template <class T>
class PEParserLoadedModuleT :
    public PEParserT<T>
{
public:
};

#endif

LIBPE_NAMESPACE_END