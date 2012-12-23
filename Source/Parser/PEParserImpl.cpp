#include "stdafx.h"
#include "Parser/PEParserImpl.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserT<T>::Create(PEParserType nType)
{
    switch(nType) {
    case PE_PARSER_TYPE_DISK_FILE:
        return new PEParserDiskFileT<T>;
    }

    return NULL;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetRVAFromAddressField(LibPEAddressT(T) nAddress)
{
    return nAddress;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetRawOffsetFromAddressField(LibPEAddressT(T) nAddress)
{
    return GetFOAFromRVA(nAddress);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, Create);

LIBPE_NAMESPACE_END