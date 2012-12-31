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
    virtual PEAddress GetRawOffsetFromAddressField(PEAddress nAddress) { return GetFOAFromRVA(nAddress); }
    virtual PEAddress GetRVAFromAddressField(PEAddress nAddress) { return nAddress; }
    virtual PEAddress GetFOAFromAddressField(PEAddress nAddress) { return GetFOAFromRVA(nAddress); }
    virtual PEAddress GetRawOffsetFromRVA(PEAddress nRVA) { return GetFOAFromRVA(nRVA); }
    virtual PEAddress GetRawOffsetFromFOA(PEAddress nFOA) { return nFOA; }
    virtual PEAddress GetRVAFromRawOffset(PEAddress nRawOffset) { return GetFOAFromRVA(nRawOffset); }
    virtual PEAddress GetFOAFromRawOffset(PEAddress nRawOffset) { return nRawOffset; }
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