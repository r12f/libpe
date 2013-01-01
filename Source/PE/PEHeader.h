#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEDosHeaderT :
    public IPEDosHeader,
    public PEElementT<T>
{
public:
    PEDosHeaderT() {}
    virtual ~PEDosHeaderT() {}

    DECLARE_PE_ELEMENT(LibPERawDosHeaderT(T))

    LIBPE_FIELD_ACCESSOR_EX(UINT16, Magic, e_magic)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Cblp, e_cblp)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Cp, e_cp)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Crlc, e_crlc)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Cparhdr, e_cparhdr)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Minalloc, e_minalloc)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Maxalloc, e_maxalloc)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Ss, e_ss)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Sp, e_sp)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Csum, e_csum)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Ip, e_ip)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Cs, e_cs)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Lfarlc, e_lfarlc)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Ovno, e_ovno)
    LIBPE_ARRAY_FIELD_ACCESSOR_EX(UINT16, Res, e_res, 4)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Oemid, e_oemid)
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Oeminfo, e_oeminfo)
    LIBPE_ARRAY_FIELD_ACCESSOR_EX(UINT16, Res2, e_res2, 10)
    LIBPE_FIELD_ACCESSOR_EX(UINT32, Lfanew, e_lfanew)
};

template <class T>
class PENtHeadersT :
    public IPENtHeaders,
    public PEElementT<T>
{
public:
    PENtHeadersT() {}
    virtual ~PENtHeadersT() {}

    DECLARE_PE_ELEMENT(LibPERawNtHeadersT(T))

    void InnerSetFileHeader(IPEFileHeader *pFileHeader) { m_pFileHeader = pFileHeader; }
    void InnerSetOptionalHeader(IPEOptionalHeader *pOptionalHeader) { m_pOptionalHeader = pOptionalHeader; }

    LIBPE_FIELD_ACCESSOR(UINT32, Signature)
    virtual HRESULT LIBPE_CALLTYPE GetFileHeader(IPEFileHeader **ppFileHeader) { return m_pFileHeader.CopyTo(ppFileHeader); }
    virtual HRESULT LIBPE_CALLTYPE GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader) { return m_pOptionalHeader.CopyTo(ppOptionalHeader); }
    
private:
    LibPEPtr<IPEFileHeader>     m_pFileHeader;
    LibPEPtr<IPEOptionalHeader> m_pOptionalHeader;
};

template <class T>
class PEFileHeaderT :
    public IPEFileHeader,
    public PEElementT<T>
{
public:
    PEFileHeaderT() {}
    virtual ~PEFileHeaderT() {}

    DECLARE_PE_ELEMENT(LibPERawFileHeaderT(T))

    LIBPE_FIELD_ACCESSOR(UINT16, Machine)
    LIBPE_FIELD_ACCESSOR(UINT16, NumberOfSections)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT32, PointerToSymbolTable)
    LIBPE_FIELD_ACCESSOR(UINT32, NumberOfSymbols)
    LIBPE_FIELD_ACCESSOR(UINT16, SizeOfOptionalHeader)
    LIBPE_FIELD_ACCESSOR(UINT16, Characteristics)
};

template <class T>
class PEOptionalHeaderT :
    public IPEOptionalHeader,
    public PEElementT<T>
{
public:
    PEOptionalHeaderT() {}
    virtual ~PEOptionalHeaderT() {}

    DECLARE_PE_ELEMENT(LibPERawOptionalHeaderT(T))

    LIBPE_FIELD_ACCESSOR(UINT16, Magic)
    LIBPE_FIELD_ACCESSOR(UINT8, MajorLinkerVersion)
    LIBPE_FIELD_ACCESSOR(UINT8, MinorLinkerVersion)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfCode)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfInitializedData)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfUninitializedData)
    LIBPE_FIELD_ACCESSOR(UINT32, AddressOfEntryPoint)
    LIBPE_FIELD_ACCESSOR(UINT32, BaseOfCode)
    LIBPE_FIELD_ACCESSOR(UINT64, ImageBase)
    LIBPE_FIELD_ACCESSOR(UINT32, SectionAlignment)
    LIBPE_FIELD_ACCESSOR(UINT32, FileAlignment)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorOperatingSystemVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorOperatingSystemVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorImageVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorImageVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorSubsystemVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorSubsystemVersion)
    LIBPE_FIELD_ACCESSOR(UINT32, Win32VersionValue)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfImage)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfHeaders)
    LIBPE_FIELD_ACCESSOR(UINT32, CheckSum)
    LIBPE_FIELD_ACCESSOR(UINT16, Subsystem)
    LIBPE_FIELD_ACCESSOR(UINT16, DllCharacteristics)
    LIBPE_FIELD_ACCESSOR(UINT64, SizeOfStackReserve)
    LIBPE_FIELD_ACCESSOR(UINT64, SizeOfStackCommit)
    LIBPE_FIELD_ACCESSOR(UINT64, SizeOfHeapReserve)
    LIBPE_FIELD_ACCESSOR(UINT64, SizeOfHeapCommit)
    LIBPE_FIELD_ACCESSOR(UINT32, LoaderFlags)
    LIBPE_FIELD_ACCESSOR(UINT32, NumberOfRvaAndSizes)
};

LIBPE_NAMESPACE_END