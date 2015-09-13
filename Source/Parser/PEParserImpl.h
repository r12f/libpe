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

    PEParserType GetType() override { return PE_PARSER_TYPE_DISK_FILE; }
    BOOL IsRawAddressVA() override { return false; }

protected:
    PEAddress GetRawOffsetFromAddressField(PEAddress nAddress) override { return GetFOAFromRVA(nAddress); }
    PEAddress GetRVAFromAddressField(PEAddress nAddress) override { return nAddress; }
    PEAddress GetFOAFromAddressField(PEAddress nAddress) override { return GetFOAFromRVA(nAddress); }
    PEAddress GetRawOffsetFromRVA(PEAddress nRVA) override { return GetFOAFromRVA(nRVA); }
    PEAddress GetRawOffsetFromFOA(PEAddress nFOA) override { return nFOA; }
    PEAddress GetRVAFromRawOffset(PEAddress nRawOffset) override { return GetFOAFromRVA(nRawOffset); }
    PEAddress GetFOAFromRawOffset(PEAddress nRawOffset) override { return nRawOffset; }
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