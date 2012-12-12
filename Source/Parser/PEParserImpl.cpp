#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PESectionHeader.h"
#include "PE/PESection.h"
#include "PE/PEExportTable.h"
#include "PE/PEExportFunction.h"
#include "PE/PEImportTable.h"
#include "PE/PEImportModule.h"
#include "PE/PEImportFunction.h"
#include "PE/PERelocationTable.h"

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
    LibPERawSectionHeaderT(T) *pRawSectionHeader = NULL;
    for(uint16_t nSectionId = 0; nSectionId < pNtHeaders->FileHeader.NumberOfSections; ++nSectionId) {
        nSectionHeaderOffset = nStartSectionHeaderOffset + nSectionId * sizeof(LibPERawSectionHeaderT(T));
        pRawSectionHeader = (LibPERawSectionHeaderT(T) *)m_pLoader->GetBuffer(nSectionHeaderOffset, sizeof(LibPERawSectionHeaderT(T)));
        if(NULL == pRawSectionHeader) {
            return ERR_FAIL;
        }

        LibPEPtr<PESectionHeaderT<T>> pSectionHeader = new PESectionHeaderT<T>();
        if(NULL == pSectionHeader) {
            return ERR_NO_MEM;
        }

        pSectionHeader->SetParser(this);
        pSectionHeader->SetPEFile(m_pFile);
        pSectionHeader->SetRVA(nSectionHeaderOffset);
        pSectionHeader->SetSizeInMemory(sizeof(LibPERawOptionalHeaderT(T)));
        pSectionHeader->SetFOA(nSectionHeaderOffset);
        pSectionHeader->SetSizeInFile(sizeof(LibPERawOptionalHeaderT(T)));
        pSectionHeader->SetRawSectionHeader(pRawSectionHeader);

        pSectionHeaders->push_back(pSectionHeader.p);
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
PEParserDiskFileT<T>::ParseExportTable(IPEExportTableT<T> **ppExportTable)
{
    LIBPE_ASSERT_RET(NULL != ppExportTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppExportTable = NULL;

    LibPERawDataDirectoryT(T) *pDataDirectory = GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_EXPORT);
    if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
        return ERR_FAIL;
    }

    LibPEAddressT(T) nExportTableRVA = pDataDirectory->VirtualAddress;
    LibPEAddressT(T) nExportTableFOA = GetFOAFromRVA(nExportTableRVA);

    LibPEPtr<PEExportTableT<T>> pExportTable = new PEExportTableT<T>();
    if(NULL == pExportTable) {
        return ERR_NO_MEM;
    }

    pExportTable->SetParser(this);
    pExportTable->SetPEFile(m_pFile);
    pExportTable->SetRVA(nExportTableRVA);
    pExportTable->SetSizeInMemory(pDataDirectory->Size);
    pExportTable->SetFOA(nExportTableFOA);
    pExportTable->SetSizeInFile(pDataDirectory->Size);

    LibPERawExportDirectory(T) *pExportDirectory = (LibPERawExportDirectory(T) *)m_pLoader->GetBuffer(nExportTableFOA, pDataDirectory->Size);
    if(NULL == pExportDirectory) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nFunctionListFOA = GetFOAFromRVA(pExportDirectory->AddressOfFunctions);
    LibPEAddressT(T) nNameListFOA = GetFOAFromRVA(pExportDirectory->AddressOfNames);
    LibPEAddressT(T) nNameOrdinalListFOA = GetFOAFromRVA(pExportDirectory->AddressOfNameOrdinals);

    LibPEAddressT(T) *pFunctionList = (LibPEAddressT(T) *)m_pLoader->GetBuffer(nFunctionListFOA, pExportDirectory->NumberOfFunctions * sizeof(LibPEAddressT(T)));
    LibPEAddressT(T) *pNameList = (LibPEAddressT(T) *)m_pLoader->GetBuffer(nNameListFOA, pExportDirectory->NumberOfNames * sizeof(LibPEAddressT(T)));
    uint16_t *pNameOrdinalList = (uint16_t *)m_pLoader->GetBuffer(nNameOrdinalListFOA, pExportDirectory->NumberOfFunctions * sizeof(uint16_t));

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, ERR_NO_MEM);

    pExportTable->SetRawFunctionList(pFunctionList);
    pExportTable->SetRawNameList(pNameList);
    pExportTable->SetRawNameOrdinalList(pNameOrdinalList);
    
    if(!pExportTable->PrepareForUsing()) {
        return ERR_FAIL;
    }

    *ppExportTable = pExportTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseExportFunction(IPEExportTableT<T> *pExportTable, uint32_t nIndex, IPEExportFunctionT<T> **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != pExportTable && NULL != ppFunction, ERR_POINTER);

    PEExportTableT<T> *pRawExportTable = static_cast<PEExportTableT<T> *>(pExportTable);
    LibPEAddressT(T) *pFunctionList = pRawExportTable->GetRawFunctionList();
    LibPEAddressT(T) *pNameList = pRawExportTable->GetRawNameList();
    uint16_t *pNameOrdinalList = pRawExportTable->GetRawNameOrdinalList();

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, ERR_FAIL);

    LibPEAddressT(T) nFunctionRVA = pFunctionList[nIndex];
    LibPEAddressT(T) nNameRVA = (nIndex < pRawExportTable->GetExportFunctionCount()) ? pNameList[nIndex] : 0;
    uint16_t nNameOrdinal = pNameOrdinalList[nIndex];

    LibPEPtr<PEExportFunctionT<T>> pFunction = new PEExportFunctionT<T>();
    if(NULL == pFunction) {
        return ERR_NO_MEM;
    }

    pFunction->SetParser(this);
    pFunction->SetPEFile(m_pFile);
    pFunction->SetRVA(nFunctionRVA);
    pFunction->SetSizeInMemory(0);
    pFunction->SetSizeInFile(0);
    pFunction->SetRawHint(nNameOrdinal);

    if(nNameRVA != 0) {
        LibPEAddressT(T) nNameFOA = GetFOAFromRVA(nNameRVA);
        const char *pName = m_pLoader->GetBuffer(nNameFOA, 256);
        pFunction->SetRawName(pName);
    }

    *ppFunction = pFunction.Detach();

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
PEParserDiskFileT<T>::ParseImportFunction(LibPERawImportDescriptor(T) *pImportDescriptor, LibPERawThunkData(T) *pThunkData, IPEImportFunctionT<T> **ppFunction)
{
    // TODO: We should deal with bound data and unbound data here.
    LIBPE_ASSERT_RET(NULL != pImportDescriptor && NULL != pThunkData && NULL != ppFunction, ERR_POINTER);

    *ppFunction = NULL;

    LibPEAddressT(T) nRawImportFunctionRVA = pThunkData->u1.AddressOfData;
    LibPEAddressT(T) nRawImportFunctionFOA = GetFOAFromRVA(nRawImportFunctionRVA);
    if(0 == nRawImportFunctionFOA) {
        return ERR_FAIL;
    }

    LibPERawImportByName(T) *pRawImportFunction = (LibPERawImportByName(T) *)m_pLoader->GetBuffer(nRawImportFunctionFOA, sizeof(LibPERawImportByName(T)));

    LibPEPtr<PEImportFunctionT<T>> pFunction = new PEImportFunctionT<T>();
    if(NULL == pFunction) {
        return ERR_NO_MEM;
    }

    pFunction->SetParser(this);
    pFunction->SetPEFile(m_pFile);
    pFunction->SetRVA(nRawImportFunctionRVA);
    pFunction->SetSizeInMemory(sizeof(IMAGE_IMPORT_BY_NAME));
    pFunction->SetFOA(nRawImportFunctionFOA);
    pFunction->SetSizeInFile(sizeof(IMAGE_IMPORT_BY_NAME));
    pFunction->SetRawThunkData(pThunkData);
    pFunction->SetRawImportByName(pRawImportFunction);
    pFunction->SetRawName((const char *)pRawImportFunction->Name);

    *ppFunction = pFunction.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseResourceTable(IPEResourceTableT<T> **ppResourceTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppRelocationTable = NULL;

    LibPERawDataDirectoryT(T) *pDataDirectory = GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IMPORT);
    if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
        return ERR_FAIL;
    }

    LibPEAddressT(T) nRelocationTableRVA = pDataDirectory->VirtualAddress;
    LibPEAddressT(T) nRelocationTableFOA = GetFOAFromRVA(nRelocationTableRVA);
    if(0 == nRelocationTableFOA) {
        return ERR_FAIL;
    }

    LibPEPtr<PERelocationTableT<T>> pRelocationTable = new PERelocationTableT<T>();
    if(NULL == pRelocationTable) {
        return ERR_NO_MEM;
    }

    pRelocationTable->SetParser(this);
    pRelocationTable->SetPEFile(m_pFile);
    pRelocationTable->SetRVA(nRelocationTableRVA);
    pRelocationTable->SetSizeInMemory(pDataDirectory->Size);
    pRelocationTable->SetFOA(nRelocationTableFOA);
    pRelocationTable->SetSizeInFile(pDataDirectory->Size);

    LibPERawBaseRelocation(T) *pRawRelocationTable = (LibPERawBaseRelocation(T) *)m_pLoader->GetBuffer(nRelocationTableFOA, pDataDirectory->Size);
    if(NULL == pRawRelocationTable) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nRelocationPageRVA = nRelocationTableRVA;
    LibPEAddressT(T) nRelocationPageFOA = nRelocationTableFOA;
    while(0 != pRawRelocationTable->VirtualAddress) {
        LibPEAddressT(T) nRelocationAddressRVABase = pRawRelocationTable->VirtualAddress;

        LibPEAddressT(T) nRelocationItemRVA = nRelocationPageRVA + sizeof(LibPERawBaseRelocation(T));
        LibPEAddressT(T) nRelocationItemFOA = nRelocationPageFOA + sizeof(LibPERawBaseRelocation(T));
        uint32_t nBlockIndex = 0, nBlockCount = pRawRelocationTable->SizeOfBlock;
        uint16_t *pBlockList = (uint16_t *)(&pRawRelocationTable[1]);
        while(nBlockIndex < nBlockCount) {
            LibPEPtr<PERelocationItemT<T>> pRelocationItem = new PERelocationItemT<T>();
            if(NULL == pRelocationItem) {
                return ERR_NO_MEM;
            }

            pRelocationItem->SetParser(this);
            pRelocationItem->SetPEFile(m_pFile);
            pRelocationItem->SetRVA(nRelocationItemRVA);
            pRelocationItem->SetSizeInMemory(sizeof(uint16_t));
            pRelocationItem->SetFOA(nRelocationItemFOA);
            pRelocationItem->SetSizeInFile(sizeof(uint16_t));

            uint16_t nRelocateFlag = (pBlockList[nBlockIndex] & 0xF000);
            LibPEAddressT(T) nRelocationAddressRVA = nRelocationAddressRVABase + pBlockList[nBlockIndex] & 0x0FFF;
            pRelocationItem->InnerSetRelocateFlag(nRelocateFlag);
            pRelocationItem->InnerSetAddressRVA(nRelocationAddressRVA);

            pRelocationTable->InnerAddRelocationItem(pRelocationItem);

            nRelocationItemRVA += sizeof(uint16_t);
            nRelocationItemFOA += sizeof(uint16_t);
            ++nBlockIndex;
        }

        nRelocationPageRVA = nRelocationItemRVA;
        nRelocationPageFOA = nRelocationItemFOA;
    }

    return ERR_FAIL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseTlsTable(IPETlsTableT<T> **ppTlsTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader)
{
    return ERR_NOT_IMPL;
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