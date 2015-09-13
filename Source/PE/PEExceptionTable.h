#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEExceptionTableT :
    public IPEExceptionTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEExceptionHandlerEntry>> ExceptionHandlerEntryList;

public:
    PEExceptionTableT() {}
    ~PEExceptionTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawRuntimeFunctionEntry(T));

    void InnerSetExceptionHandlerCount(_In_ UINT32 nCount) throw() { m_vExceptionHandlerEntries.resize(nCount); }

    UINT32 LIBPE_CALLTYPE GetExceptionHandlerCount() override;
    HRESULT LIBPE_CALLTYPE GetExceptionHandlerEntryByIndex(_In_ UINT32 nIndex, _Outptr_ IPEExceptionHandlerEntry **ppExceptionHandlerEntry) override;
    HRESULT LIBPE_CALLTYPE GetExceptionHandlerByIndex(_In_ UINT32 nIndex, _Outptr_ IPEExceptionHandler **ppExceptionHandler) override;

private:
    ExceptionHandlerEntryList m_vExceptionHandlerEntries;
};

template <class T>
class PEExceptionHandlerEntryT :
    public IPEExceptionHandlerEntry,
    public PEElementT<T>
{
public:
    PEExceptionHandlerEntryT() {}
    ~PEExceptionHandlerEntryT() override {}

    DECLARE_PE_ELEMENT(LibPERawRuntimeFunctionEntry(T));

    HRESULT LIBPE_CALLTYPE GetExceptionHandler(_Outptr_ IPEExceptionHandler **ppExceptionHandler) override;

private:
    LibPEPtr<IPEExceptionHandler> m_pExceptionHandler;
};

template <class T>
class PEExceptionHandlerT :
    public IPEExceptionHandler,
    public PEElementT<T>
{
public:
    PEExceptionHandlerT() {}
    ~PEExceptionHandlerT() override {}

    DECLARE_PE_ELEMENT(void);
};

LIBPE_NAMESPACE_END