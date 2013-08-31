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

    void InnerSetRootDirectory(IPEResourceDirectory *pRootDirectory) {
        LIBPE_CHK_RET_VOID(NULL != pRootDirectory);
        m_pRootDirectory = pRootDirectory;
    }

    virtual HRESULT LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectory **ppDirectory);

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

    void InnerReserveEntry(UINT32 nCount)
    {
        m_vEntries.resize(nCount);
    }

    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, NumberOfNamedEntries)
    LIBPE_FIELD_ACCESSOR(UINT16, NumberOfIdEntries)

    virtual UINT32 LIBPE_CALLTYPE GetEntryCount();
    virtual HRESULT LIBPE_CALLTYPE GetEntryByIndex(UINT32 nIndex, IPEResourceDirectoryEntry **ppEntry);

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

    LIBPE_FIELD_ACCESSOR(UINT32, NameOffset)
    LIBPE_FIELD_ACCESSOR(UINT32, NameIsString)
    LIBPE_FIELD_ACCESSOR(UINT32, Name)
    LIBPE_FIELD_ACCESSOR(UINT16, Id)
    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToData)
    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToDirectory)
    LIBPE_FIELD_ACCESSOR(UINT32, DataIsDirectory)

    virtual BOOL LIBPE_CALLTYPE IsNameId();
    virtual UINT16 LIBPE_CALLTYPE GetId();

    virtual BOOL LIBPE_CALLTYPE IsNameString();
    virtual const wchar_t * LIBPE_CALLTYPE GetName();
    
    virtual BOOL LIBPE_CALLTYPE IsEntryDirectory();
    virtual HRESULT LIBPE_CALLTYPE GetDirectory(IPEResourceDirectory **ppDirectory);

    virtual BOOL LIBPE_CALLTYPE IsEntryDataEntry();
    virtual HRESULT LIBPE_CALLTYPE GetDataEntry(IPEResourceDataEntry **ppDataEntry);
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

    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToData)
    LIBPE_FIELD_ACCESSOR(UINT32, Size)
    LIBPE_FIELD_ACCESSOR(UINT32, CodePage)
    LIBPE_FIELD_ACCESSOR(UINT32, Reserved)

    virtual HRESULT LIBPE_CALLTYPE GetResource(IPEResource **ppResource);

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
};

LIBPE_NAMESPACE_END