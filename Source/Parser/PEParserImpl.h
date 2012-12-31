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

protected:
    virtual LibPEAddressT(T) GetRawOffsetFromAddressField(LibPEAddressT(T) nAddress) { return GetFOAFromRVA(nAddress); }
    virtual LibPEAddressT(T) GetRVAFromAddressField(LibPEAddressT(T) nAddress) { return nAddress; }
    virtual LibPEAddressT(T) GetFOAFromAddressField(LibPEAddressT(T) nAddress) { return GetFOAFromRVA(nAddress); }
    virtual LibPEAddressT(T) GetRawOffsetFromRVA(LibPEAddressT(T) nRVA) { return GetFOAFromRVA(nRVA); }
    virtual LibPEAddressT(T) GetRawOffsetFromFOA(LibPEAddressT(T) nFOA) { return nFOA; }
    virtual LibPEAddressT(T) GetRVAFromRawOffset(LibPEAddressT(T) nRawOffset) { return GetFOAFromRVA(nRawOffset); }
    virtual LibPEAddressT(T) GetFOAFromRawOffset(LibPEAddressT(T) nRawOffset) { return nRawOffset; }
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