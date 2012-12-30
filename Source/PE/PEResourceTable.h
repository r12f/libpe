#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEResourceTableT :
    public IPEResourceTableT<T>,
    public PEElementT<T>
{
public:
    PEResourceTableT() {}
    virtual ~PEResourceTableT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectory(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerSetRootDirectory(IPEResourceDirectoryT<T> *pRootDirectory)
    {
        LIBPE_ASSERT_RET_VOID(NULL != pRootDirectory);
        m_pRootDirectory = pRootDirectory;
    }

    virtual error_t LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectoryT<T> **ppDirectory);

private:
    LibPEPtr<IPEResourceDirectoryT<T>>  m_pRootDirectory;
};

template <class T>
class PEResourceDirectoryT :
    public IPEResourceDirectoryT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEResourceDirectoryEntryT<T>>> EntryList;

public:
    PEResourceDirectoryT() {}
    virtual ~PEResourceDirectoryT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectory(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerReserveEntry(uint32_t nCount)
    {
        m_vEntries.resize(nCount);
    }

    virtual uint32_t LIBPE_CALLTYPE GetEntryCount();
    virtual error_t LIBPE_CALLTYPE GetEntryByIndex(uint32_t nIndex, IPEResourceDirectoryEntryT<T> **ppEntry);

private:
    EntryList   m_vEntries;
};

template <class T>
class PEResourceDirectoryEntryT :
    public IPEResourceDirectoryEntryT<T>,
    public PEElementT<T>
{
public:
    PEResourceDirectoryEntryT() {}
    virtual ~PEResourceDirectoryEntryT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectoryEntry(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    virtual bool_t LIBPE_CALLTYPE IsNameId();
    virtual uint16_t LIBPE_CALLTYPE GetId();

    virtual bool_t LIBPE_CALLTYPE IsNameString();
    virtual const wchar_t * LIBPE_CALLTYPE GetName();
    
    virtual bool_t LIBPE_CALLTYPE IsEntryDirectory();
    virtual error_t LIBPE_CALLTYPE GetDirectory(IPEResourceDirectoryT<T> **ppDirectory);

    virtual bool_t LIBPE_CALLTYPE IsEntryDataEntry();
    virtual error_t LIBPE_CALLTYPE GetDataEntry(IPEResourceDataEntryT<T> **ppDataEntry);
};

template <class T>
class PEResourceDataEntryT :
    public IPEResourceDataEntryT<T>,
    public PEElementT<T>
{
public:
    PEResourceDataEntryT() {}
    virtual ~PEResourceDataEntryT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDataEntry(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    virtual error_t LIBPE_CALLTYPE GetResource(IPEResourceT<T> **ppResource);

private:
    LibPEPtr<IPEResourceT<T>> m_pResource;
};

template <class T>
class PEResourceT :
    public IPEResourceT<T>,
    public PEElementT<T>
{
public:
    PEResourceT() {}
    virtual ~PEResourceT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()
};

LIBPE_NAMESPACE_END