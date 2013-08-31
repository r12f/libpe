#include "stdafx.h"
#include "PE/PEExceptionTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEExceptionTableT<T>::GetExceptionHandlerCount()
{
    return (UINT32)m_vExceptionHandlerEntries.size();
}

template <class T>
HRESULT
PEExceptionTableT<T>::GetExceptionHandlerEntryByIndex(UINT32 nIndex, IPEExceptionHandlerEntry **ppExceptionHandlerEntry)
{
    UINT32 nExceptionHandlerCount = GetExceptionHandlerCount();
    LIBPE_CHK(nIndex < nExceptionHandlerCount, E_BOUNDS);

    if(NULL == m_vExceptionHandlerEntries[nIndex]) {
        LIBPE_CHK(NULL != m_pParser, E_UNEXPECTED);
        LIBPE_CHK_HR(m_pParser->ParseExceptionHandlerEntry(this, nIndex, &m_vExceptionHandlerEntries[nIndex]));
    }

    return m_vExceptionHandlerEntries[nIndex].CopyTo(ppExceptionHandlerEntry);
}

template <class T>
HRESULT
PEExceptionTableT<T>::GetExceptionHandlerByIndex(UINT32 nIndex, IPEExceptionHandler **ppExceptionHandler)
{
    LibPEPtr<IPEExceptionHandlerEntry> pExceptionHandlerEntry;
    LIBPE_CHK_HR(GetExceptionHandlerEntryByIndex(nIndex, &pExceptionHandlerEntry));
    LIBPE_CHK(NULL != pExceptionHandlerEntry, E_UNEXPECTED);

    return pExceptionHandlerEntry->GetExceptionHandler(ppExceptionHandler);
}

template <class T>
HRESULT
PEExceptionHandlerEntryT<T>::GetExceptionHandler(IPEExceptionHandler **ppExceptionHandler)
{
    LIBPE_CHK(NULL != ppExceptionHandler, E_POINTER);

    if(NULL == m_pExceptionHandler) {
        LIBPE_CHK(NULL != m_pParser, E_UNEXPECTED);
        LIBPE_CHK_HR(m_pParser->ParseExceptionHandler(this, &m_pExceptionHandler));
    }

    return m_pExceptionHandler.CopyTo(ppExceptionHandler);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExceptionTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExceptionHandlerEntryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExceptionHandlerT);

LIBPE_NAMESPACE_END
