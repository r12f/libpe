#include "stdafx.h"
#include "Parser/PEParserImpl.h"

LIBPE_NAMESPACE_BEGIN

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

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetRawOffsetFromRVA(LibPEAddressT(T) nRVA)
{
    return GetFOAFromRVA(nRVA);
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetRawOffsetFromFOA(LibPEAddressT(T) nFOA)
{
    return nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserDiskFileT);

LIBPE_NAMESPACE_END