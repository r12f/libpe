#include "stdafx.h"
#include "PEImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEImportTableT<T>::GetModuleCount()
{
    LIBPE_CHK_HR(EnsureDataReady());
    return (UINT32)m_vModules.size();
}

template <class T>
HRESULT
PEImportTableT<T>::GetModuleByIndex(UINT32 nModuleId, IPEImportModule **ppImportModule)
{
    LIBPE_CHK(NULL != ppImportModule, E_POINTER);
    LIBPE_CHK_HR(EnsureDataReady());

    UINT32 nModuleCount = (UINT32) m_vModules.size();
    LIBPE_CHK(nModuleId < nModuleCount, E_INVALIDARG);

    return m_vModules[nModuleId].CopyTo(ppImportModule);
}

template <class T>
HRESULT
PEImportTableT<T>::GetModuleByName(const char *pModuleName, IPEImportModule **ppImportModule)
{
    LIBPE_CHK_HR(EnsureDataReady());
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEImportTableT<T>::GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunction **ppImportFunction)
{
    LIBPE_CHK_HR(EnsureDataReady());
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEImportTableT<T>::LoadDelayedData()
{
    LIBPE_CHK(NULL != m_pParser && NULL != m_pFile, E_FAIL);
    LIBPE_CHK_HR(m_pParser->ParseAllImportModules(this));

    return __super::LoadDelayedData();
}

template <class T>
BOOL
PEImportModuleT<T>::IsBound()
{
    LibPERawImportDescriptor(T) *pImportDesc = GetRawStruct();
    LIBPE_CHK(NULL != pImportDesc, FALSE);
    return (TRUE != pImportDesc->TimeDateStamp);
}

template <class T>
UINT32
PEImportModuleT<T>::GetFunctionCount()
{
    LIBPE_CHK_HR_RET(EnsureFunctionParsed(), 0);
    return (UINT32)m_vFunctions.size();
}

template <class T>
HRESULT
PEImportModuleT<T>::GetFunctionByIndex(UINT32 nIndex, IPEImportFunction **ppFunction)
{
    LIBPE_CHK(NULL != ppFunction, E_POINTER);

    LIBPE_CHK_HR(EnsureFunctionParsed());

    UINT32 nFunctionCount = GetFunctionCount();
    LIBPE_CHK(nIndex < nFunctionCount, E_INVALIDARG);

    return m_vFunctions[nIndex].CopyTo(ppFunction);
}

template <class T>
HRESULT
PEImportModuleT<T>::GetFunctionByName(const char *pFunctionName, IPEImportFunction **ppFunction)
{
    LIBPE_CHK_HR(EnsureFunctionParsed());
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEImportModuleT<T>::GetRelatedImportAddressBlock(IPEImportAddressBlock **ppBlock)
{
    LIBPE_CHK(NULL != ppBlock, E_POINTER);

    // However, we should parse the related IAT block here. Very useful for bounded module.
    if(NULL == m_pRelatedIABlock) {
        LIBPE_CHK(NULL != m_pParser, E_FAIL);

        LibPERawImportDescriptor(T) *pImportDescriptor = GetRawStruct();
        LIBPE_CHK(NULL != pImportDescriptor, E_FAIL);

        PEAddress nImportAddressBlockRVA = pImportDescriptor->FirstThunk;
        if(0 != nImportAddressBlockRVA) {
            PEAddress nImportAddressBlockFOA = m_pParser->GetFOAFromRVA(nImportAddressBlockRVA);
            if(FAILED(m_pParser->ParseImportAddressBlock(NULL, nImportAddressBlockRVA, nImportAddressBlockFOA, &m_pRelatedIABlock)) || NULL == m_pRelatedIABlock) {
                return E_FAIL;
            }
        }
    }

    return m_pRelatedIABlock.CopyTo(ppBlock);
}

template <class T>
PERawImportByName *
PEImportFunctionT<T>::GetRawImportByName()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), NULL);
    return m_pImportByName;
}

template <class T>
PEAddress
PEImportFunctionT<T>::GetRawImportByNameRVA()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), LIBPE_INVALID_ADDRESS);
    return m_nImportByNameRVA;
}

template <class T>
PEAddress
PEImportFunctionT<T>::GetRawImportByNameFOA()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), LIBPE_INVALID_ADDRESS);
    return m_nImportByNameFOA;
}

template <class T>
PEAddress
PEImportFunctionT<T>::GetRawImportByNameSize()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), LIBPE_INVALID_SIZE);
    return m_nImportByNameSize;
}


template <class T>
const char *
PEImportFunctionT<T>::GetName()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), NULL);

    if (NULL == m_pImportByName) {
        return NULL;
    }

    return m_pImportByName->Name;
}

template <class T>
UINT16
PEImportFunctionT<T>::GetOrdinal()
{
    LIBPE_CHK_HR_RET(EnsureImportByNameParsed(), 0);

    if (NULL != m_pImportByName) {
        return m_pImportByName->Hint;
    }

    LibPERawThunkData(T) *pThunkData = GetRawStruct();
    LIBPE_CHK(NULL != pThunkData, 0);
    
    if ((PETrait<T>::ImageOrdinalFlag & pThunkData->u1.Ordinal) == 0) {
        return 0;
    }

    return (UINT16)(pThunkData->u1.Ordinal & 0xFFFF);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportModuleT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportFunctionT);

LIBPE_NAMESPACE_END
