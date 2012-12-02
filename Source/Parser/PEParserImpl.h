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
    virtual error_t ParsePEBasicInfo();

protected:
    virtual PEAddressT<T> GetRVAFromRawAddress(PEAddressT<T> nRawAddress);
    virtual PEAddressT<T> GetVAFromRawAddress(PEAddressT<T> nRawAddress);
    virtual PEAddressT<T> GetFOAFromRawAddress(PEAddressT<T> nRawAddress);
};

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