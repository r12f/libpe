#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PESectionHeader.h"
#include "PE/PESection.h"
#include "PE/PEImportTable.h"
#include "PE/PEImportModule.h"
#include "PE/PEImportFunction.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserT<T>::Create(PEParserType nType)
{
    switch(nType) {
    case PE_PARSER_TYPE_DISK_FILE:
        return new PEParserDiskFileT<T>;
    }

    return NULL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, LibPERawNtHeadersT(T) **ppNtHeaders, SectionHeaderList *pSectionHeaders)
{
    LIBPE_ASSERT_RET(NULL != ppDosHeader && NULL != ppNtHeaders && NULL != pSectionHeaders, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    // Parse PE header
    LibPERawDosHeaderT(T) *pDosHeader = (LibPERawDosHeaderT(T) *)m_pLoader->GetBuffer(0, sizeof(LibPERawDosHeaderT(T)));
    LIBPE_ASSERT_RET(NULL != pDosHeader, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_DOS_SIGNATURE == pDosHeader->e_magic, ERR_FAIL);

    LibPERawNtHeadersT(T) *pNtHeaders = (LibPERawNtHeadersT(T) *)m_pLoader->GetBuffer(pDosHeader->e_lfanew, sizeof(LibPERawNtHeadersT(T)));
    LIBPE_ASSERT_RET(NULL != pNtHeaders, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_NT_SIGNATURE == pNtHeaders->Signature, ERR_FAIL);

    *ppDosHeader = pDosHeader;
    *ppNtHeaders = pNtHeaders;

    // Parse section headers
    uint32_t nSectionHeaderOffset = 0;
    uint32_t nStartSectionHeaderOffset = pDosHeader->e_lfanew + sizeof(DWORD) + sizeof(LibPERawFileHeaderT(T)) + pNtHeaders->FileHeader.SizeOfOptionalHeader;
    LibPERawSectionHeaderT(T) *pSectionHeader = NULL;
    for(uint16_t nSectionId = 0; nSectionId < pNtHeaders->FileHeader.NumberOfSections; ++nSectionId) {
        nSectionHeaderOffset = nStartSectionHeaderOffset + nSectionId * sizeof(LibPERawSectionHeaderT(T));
        pSectionHeader = (LibPERawSectionHeaderT(T) *)m_pLoader->GetBuffer(nSectionHeaderOffset, sizeof(LibPERawSectionHeaderT(T)));
        if(NULL == pSectionHeader) {
            return ERR_FAIL;
        }

        LibPEPtr<PESectionHeaderT<T>> pRawSectionHeader = new PESectionHeaderT<T>();
        LibPEPtr<PESectionT<T>> pRawSection = new PESectionT<T>();
        if(NULL == pRawSectionHeader || NULL == pRawSection) {
            return ERR_NO_MEM;
        }

        pRawSectionHeader->SetParser(this);
        pRawSectionHeader->SetPEFile(m_pFile);
        pRawSectionHeader->SetRVA(nSectionHeaderOffset);
        pRawSectionHeader->SetSizeInMemory(sizeof(LibPERawOptionalHeaderT(T)));
        pRawSectionHeader->SetFOA(nSectionHeaderOffset);
        pRawSectionHeader->SetSizeInFile(sizeof(LibPERawOptionalHeaderT(T)));
        pRawSectionHeader->SetRawSectionHeader(pSectionHeader);

        pSectionHeaders->push_back(pRawSectionHeader.p);
    }

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection)
{
    LIBPE_ASSERT_RET(NULL != pSectionHeader && NULL != ppSection, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    LibPEPtr<PESectionT<T>> pRawSection = new PESectionT<T>();
    if(NULL == pRawSection) {
        return ERR_NO_MEM;
    }

    pRawSection->SetParser(this);
    pRawSection->SetPEFile(m_pFile);
    pRawSection->SetRVA(pSectionHeader->VirtualAddress);
    pRawSection->SetSizeInMemory(pSectionHeader->SizeOfRawData);
    pRawSection->SetFOA(pSectionHeader->PointerToRawData);
    pRawSection->SetSizeInFile(pSectionHeader->SizeOfRawData);
    pRawSection->SetRawSectionHeader(pSectionHeader);

    *ppSection = pRawSection.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportTable(IPEImportTableT<T> **ppImportTable)
{
    LIBPE_ASSERT_RET(NULL != ppImportTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppImportTable = NULL;

    LibPERawDataDirectoryT(T) *pDataDirectory = GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IMPORT);
    if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
        return ERR_FAIL;
    }

    LibPEAddressT(T) nImportDescRVA = pDataDirectory->VirtualAddress;
    LibPEAddressT(T) nImportDescFOA = GetFOAFromRVA(nImportDescRVA);
    if(0 == nImportDescFOA) {
        return ERR_FAIL;
    }

    LibPEPtr<PEImportTableT<T>> pImportTable = new PEImportTableT<T>();
    if(NULL == pImportTable) {
        return ERR_NO_MEM;
    }

    pImportTable->SetParser(this);
    pImportTable->SetPEFile(m_pFile);
    pImportTable->SetRVA(nImportDescRVA);
    pImportTable->SetSizeInMemory(pDataDirectory->Size);
    pImportTable->SetFOA(nImportDescFOA);
    pImportTable->SetSizeInFile(pDataDirectory->Size);

    LibPERawImportDescriptor(T) *pImportDesc = (LibPERawImportDescriptor(T) *)m_pLoader->GetBuffer(nImportDescFOA, pDataDirectory->Size);
    if(NULL == pImportDesc) {
        return ERR_NO_MEM;
    }

    while(0 != pImportDesc->Characteristics && 0 != pImportDesc->Name) {
        pImportTable->AddImportDescriptor(nImportDescRVA, nImportDescFOA, pImportDesc);
        ++pImportDesc;
        nImportDescRVA += sizeof(LibPERawImportDescriptor(T));
        nImportDescFOA += sizeof(LibPERawImportDescriptor(T));
    }

    *ppImportTable = pImportTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportModule(LibPEAddressT(T) nImportDescRVA, LibPEAddressT(T) nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModuleT<T> **ppImportModule)
{
    LIBPE_ASSERT_RET(NULL != pImportDescriptor && NULL != ppImportModule, ERR_POINTER);

    *ppImportModule = NULL;

    LibPEAddressT(T) nImportNameFOA = GetFOAFromRVA(pImportDescriptor->Name);
    if(0 == nImportNameFOA) {
        return ERR_FAIL;
    }

    const char *pImportName = (const char *)m_pLoader->GetBuffer(nImportNameFOA, 256);

    LibPEPtr<PEImportModuleT<T>> pImportModule = new PEImportModuleT<T>();
    LIBPE_ASSERT_RET(NULL != pImportModule, ERR_NO_MEM);

    pImportModule->SetParser(this);
    pImportModule->SetPEFile(m_pFile);
    pImportModule->SetRVA(nImportDescRVA);
    pImportModule->SetSizeInMemory(sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->SetFOA(nImportDescFOA);
    pImportModule->SetSizeInFile(sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->SetRawName(pImportName);

    // First bridge to IMAGE_IMPORT_BY_NAME
    LibPEAddressT(T) nImportThunkRVA = pImportDescriptor->OriginalFirstThunk;
    LibPEAddressT(T) nImportThunkFOA = GetFOAFromRVA(nImportThunkRVA);
    if(0 == nImportThunkFOA) {
        return ERR_FAIL;
    }

    for(;;) {
        LibPERawThunkData(T) *pThunkData = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nImportThunkFOA, sizeof(LibPERawThunkData(T)));
        if(NULL == pThunkData || 0 == pThunkData->u1.AddressOfData) {
            break;
        }
        pImportModule->AddImportFunctionThunk(pThunkData);
        nImportThunkRVA += sizeof(LibPERawThunkData(T));
        nImportThunkFOA += sizeof(LibPERawThunkData(T));
    }

    *ppImportModule = pImportModule.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportFunction(LibPERawThunkData(T) *pThunkData, IPEImportFunctionT<T> **ppFunction)
{
    // TODO: We should deal with bound data and unbound data here.
    LIBPE_ASSERT_RET(NULL != pThunkData && NULL != ppFunction, ERR_POINTER);

    *ppFunction = NULL;

    LibPEAddressT(T) nImportRVA = pThunkData->u1.AddressOfData;
    LibPEAddressT(T) nImportFOA = GetFOAFromRVA(nImportRVA);
    if(0 == nImportFOA) {
        return ERR_FAIL;
    }

    LibPERawImportByName(T) *pImport = (LibPERawImportByName(T) *)m_pLoader->GetBuffer(nImportFOA, sizeof(LibPERawImportByName(T)));

    LibPEPtr<PEImportFunctionT<T>> pFunction = new PEImportFunctionT<T>();
    if(NULL == pFunction) {
        return ERR_NO_MEM;
    }

    pFunction->SetParser(this);
    pFunction->SetPEFile(m_pFile);
    pFunction->SetRVA(nImportRVA);
    pFunction->SetSizeInMemory(sizeof(IMAGE_IMPORT_BY_NAME));
    pFunction->SetFOA(nImportFOA);
    pFunction->SetSizeInFile(sizeof(IMAGE_IMPORT_BY_NAME));
    pFunction->SetRawThunkData(pThunkData);
    pFunction->SetRawImportByName(pImport);
    pFunction->SetRawName((const char *)pImport->Name);

    *ppFunction = pFunction.Detach();

    return ERR_OK;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromRVA(LibPEAddressT(T) nRVA)
{
    return nRVA;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromVA(LibPEAddressT(T) nVA)
{
    return nVA;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromFOA(LibPEAddressT(T) nFOA)
{
    return nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, Create);

LIBPE_NAMESPACE_END