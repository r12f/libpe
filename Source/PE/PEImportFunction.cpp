#include "stdafx.h"
#include "PEImportFunction.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
LibPERawThunkData(T) *  
PEImportFunctionT<T>::GetRawThunkData()
{
    return m_pThunkData;
}

template <class T>
const char *  
PEImportFunctionT<T>::GetName()
{
    return m_pFunctionName;
}

template <class T>
uint16_t  
PEImportFunctionT<T>::GetOrdinal()
{
    return m_pImportByName->Hint;
}

template <class T>
LibPEAddressT(T)  
PEImportFunctionT<T>::GetEntry()
{
    return NULL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportFunction);

LIBPE_NAMESPACE_END