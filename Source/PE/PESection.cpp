#include "stdafx.h"
#include "PE/PESection.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PESectionT<T>::GetName(char *pName, int32_t nMaxSize)
{
    return ERR_OK;
}

template <class T>
error_t
PESectionT<T>::SetName(const char *pName)
{
    return ERR_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESection);

LIBPE_NAMESPACE_END