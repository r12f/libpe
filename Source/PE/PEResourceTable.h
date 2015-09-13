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
    ~PEResourceTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectory(T))

    void InnerSetRootDirectory(IPEResourceDirectory *pRootDirectory) {
        LIBPE_CHK_RET_VOID(NULL != pRootDirectory);
        m_pRootDirectory = pRootDirectory;
    }

    HRESULT LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectory **ppDirectory) override;

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
    ~PEResourceDirectoryT() override {}

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

    UINT32 LIBPE_CALLTYPE GetEntryCount() override;
    HRESULT LIBPE_CALLTYPE GetEntryByIndex(UINT32 nIndex, IPEResourceDirectoryEntry **ppEntry) override;

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
    ~PEResourceDirectoryEntryT() override {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectoryEntry(T))

    LIBPE_FIELD_ACCESSOR(UINT32, NameOffset)
    LIBPE_FIELD_ACCESSOR(UINT32, NameIsString)
    LIBPE_FIELD_ACCESSOR(UINT32, Name)
    LIBPE_FIELD_ACCESSOR(UINT16, Id)
    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToData)
    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToDirectory)
    LIBPE_FIELD_ACCESSOR(UINT32, DataIsDirectory)

    BOOL LIBPE_CALLTYPE IsNameId() override;
    UINT16 LIBPE_CALLTYPE GetId() override;

    BOOL LIBPE_CALLTYPE IsNameString() override;
    const wchar_t * LIBPE_CALLTYPE GetName() override;
    
    BOOL LIBPE_CALLTYPE IsEntryDirectory() override;
    HRESULT LIBPE_CALLTYPE GetDirectory(IPEResourceDirectory **ppDirectory) override;

    BOOL LIBPE_CALLTYPE IsEntryDataEntry() override;
    HRESULT LIBPE_CALLTYPE GetDataEntry(IPEResourceDataEntry **ppDataEntry) override;
};

template <class T>
class PEResourceDataEntryT :
    public IPEResourceDataEntry,
    public PEElementT<T>
{
public:
    PEResourceDataEntryT() {}
    ~PEResourceDataEntryT() override {}

    DECLARE_PE_ELEMENT(LibPERawResourceDataEntry(T))

    LIBPE_FIELD_ACCESSOR(UINT32, OffsetToData)
    LIBPE_FIELD_ACCESSOR(UINT32, Size)
    LIBPE_FIELD_ACCESSOR(UINT32, CodePage)
    LIBPE_FIELD_ACCESSOR(UINT32, Reserved)

    HRESULT LIBPE_CALLTYPE GetResource(IPEResource **ppResource) override;

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
    ~PEResourceT() override {}

    DECLARE_PE_ELEMENT(void)
};

LIBPE_NAMESPACE_END