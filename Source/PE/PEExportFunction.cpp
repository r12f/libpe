#include "stdafx.h"
#include "PE/PEExportFunction.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
const char *
PEExportFunctionT<T>::GetName()
{
    return m_pName;
}

template <class T>
uint16_t
PEExportFunctionT<T>::GetHint()
{
    return m_nHint;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExportFunction);

LIBPE_NAMESPACE_END