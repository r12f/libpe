#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEResourceTableT :
    public IPEResourceTable,
    public PEElementT<T>
{
public:
    PEResourceTableT() {}
    virtual ~PEResourceTableT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectory(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerSetRootDirectory(IPEResourceDirectory *pRootDirectory)
    {
        LIBPE_ASSERT_RET_VOID(NULL != pRootDirectory);
        m_pRootDirectory = pRootDirectory;
    }

    virtual error_t LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectory **ppDirectory);

private:
    LibPEPtr<IPEResourceDirectory>  m_pRootDirectory;
};

template <class T>
class PEResourceDirectoryT :
    public IPEResourceDirectory,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEResourceDirectoryEntry>> EntryList;

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
    virtual error_t LIBPE_CALLTYPE GetEntryByIndex(uint32_t nIndex, IPEResourceDirectoryEntry **ppEntry);

private:
    EntryList   m_vEntries;
};

template <class T>
class PEResourceDirectoryEntryT :
    public IPEResourceDirectoryEntry,
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
    virtual error_t LIBPE_CALLTYPE GetDirectory(IPEResourceDirectory **ppDirectory);

    virtual bool_t LIBPE_CALLTYPE IsEntryDataEntry();
    virtual error_t LIBPE_CALLTYPE GetDataEntry(IPEResourceDataEntry **ppDataEntry);
};

template <class T>
class PEResourceDataEntryT :
    public IPEResourceDataEntry,
    public PEElementT<T>
{
public:
    PEResourceDataEntryT() {}
    virtual ~PEResourceDataEntryT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDataEntry(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    virtual error_t LIBPE_CALLTYPE GetResource(IPEResource **ppResource);

private:
    LibPEPtr<IPEResource> m_pResource;
};

template <class T>
class PEResourceT :
    public IPEResource,
    public PEElementT<T>
{
public:
    PEResourceT() {}
    virtual ~PEResourceT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()
};

LIBPE_NAMESPACE_END