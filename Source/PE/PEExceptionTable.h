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
    virtual ~PEExceptionTableT() {}

    DECLARE_PE_ELEMENT(LibPERawRuntimeFunctionEntry(T));

    void InnerSetExceptionHandlerCount(UINT32 nCount) throw() { m_vExceptionHandlerEntries.resize(nCount); }

    virtual UINT32 LIBPE_CALLTYPE GetExceptionHandlerCount();
    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandlerEntryByIndex(UINT32 nIndex, IPEExceptionHandlerEntry **ppExceptionHandlerEntry);
    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandlerByIndex(UINT32 nIndex, IPEExceptionHandler **ppExceptionHandler);

private:
    ExceptionHandlerEntryList   m_vExceptionHandlerEntries;
};

template <class T>
class PEExceptionHandlerEntryT :
    public IPEExceptionHandlerEntry,
    public PEElementT<T>
{
public:
    PEExceptionHandlerEntryT() {}
    virtual ~PEExceptionHandlerEntryT() {}

    DECLARE_PE_ELEMENT(LibPERawRuntimeFunctionEntry(T));

    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandler(IPEExceptionHandler **ppExceptionHandler);

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
    virtual ~PEExceptionHandlerT() {}

    DECLARE_PE_ELEMENT(void);
};

LIBPE_NAMESPACE_END