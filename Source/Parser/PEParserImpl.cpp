#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PESectionHeader.h"
#include "PE/PESection.h"
#include "PE/PEExportTable.h"
#include "PE/PEImportTable.h"
#include "PE/PERelocationTable.h"
#include "PE/PEImportAddressTable.h"

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

    LibPEAddressT(T) nExportTableRVA = 0, nExportTableFOA = 0, nExportTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntryInfo(IMAGE_DIRECTORY_ENTRY_EXPORT, nExportTableRVA, nExportTableFOA, nExportTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEExportTableT<T>> pExportTable = new PEExportTableT<T>();
    if(NULL == pExportTable) {
        return ERR_NO_MEM;
    }

    pExportTable->SetParser(this);
    pExportTable->SetPEFile(m_pFile);
    pExportTable->SetRVA(nExportTableRVA);
    pExportTable->SetSizeInMemory(nExportTableSize);
    pExportTable->SetFOA(nExportTableFOA);
    pExportTable->SetSizeInFile(nExportTableSize);

    LibPERawExportDirectory(T) *pExportDirectory = pExportTable->GetRawStruct();
    if(NULL == pExportDirectory) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nFunctionListFOA = GetFOAFromRVA(pExportDirectory->AddressOfFunctions);
    LibPEAddressT(T) nNameListFOA = GetFOAFromRVA(pExportDirectory->AddressOfNames);
    LibPEAddressT(T) nNameOrdinalListFOA = GetFOAFromRVA(pExportDirectory->AddressOfNameOrdinals);

    uint32_t *pFunctionList = (uint32_t *)m_pLoader->GetBuffer(nFunctionListFOA, pExportDirectory->NumberOfFunctions * sizeof(LibPEAddressT(T)));
    uint32_t *pNameList = (uint32_t *)m_pLoader->GetBuffer(nNameListFOA, pExportDirectory->NumberOfNames * sizeof(LibPEAddressT(T)));
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
    uint32_t *pFunctionList = pRawExportTable->GetRawFunctionList();
    uint32_t *pNameList = pRawExportTable->GetRawNameList();
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
        uint64_t nNameBufferSize = 0;
        const char *pName = m_pLoader->GetAnsiString(nNameFOA, nNameBufferSize);
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

    LibPEAddressT(T) nImportTableRVA = 0, nImportTableFOA = 0, nImportTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntryInfo(IMAGE_DIRECTORY_ENTRY_IMPORT, nImportTableRVA, nImportTableFOA, nImportTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEImportTableT<T>> pImportTable = new PEImportTableT<T>();
    if(NULL == pImportTable) {
        return ERR_NO_MEM;
    }

    pImportTable->SetParser(this);
    pImportTable->SetPEFile(m_pFile);
    pImportTable->SetRVA(nImportTableRVA);
    pImportTable->SetSizeInMemory(nImportTableSize);
    pImportTable->SetFOA(nImportTableFOA);
    pImportTable->SetSizeInFile(nImportTableSize);

    LibPERawImportDescriptor(T) *pImportDesc = pImportTable->GetRawStruct();
    if(NULL == pImportDesc) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nImportDescRVA = nImportTableRVA, nImportDescFOA = nImportTableFOA;
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

    uint64_t nNameBufferSize = 0;
    const char *pImportName = m_pLoader->GetAnsiString(nImportNameFOA, nNameBufferSize);

    LibPEPtr<PEImportModuleT<T>> pImportModule = new PEImportModuleT<T>();
    LIBPE_ASSERT_RET(NULL != pImportModule, ERR_NO_MEM);

    pImportModule->SetParser(this);
    pImportModule->SetPEFile(m_pFile);
    pImportModule->SetRVA(nImportDescRVA);
    pImportModule->SetSizeInMemory(sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->SetFOA(nImportDescFOA);
    pImportModule->SetSizeInFile(sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->SetRawName(pImportName);

    // TODO: We should deal with bound data and unbound data here.
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
    LIBPE_ASSERT_RET(NULL != pImportDescriptor && NULL != pThunkData && NULL != ppFunction, ERR_POINTER);

    *ppFunction = NULL;

    LibPEAddressT(T) nRawImportFunctionRVA = pThunkData->u1.AddressOfData;
    LibPEAddressT(T) nRawImportFunctionFOA = GetFOAFromRVA(nRawImportFunctionRVA);
    if(0 == nRawImportFunctionFOA) {
        return ERR_FAIL;
    }

    uint64_t nNameBufferSize = 0; 
    if(NULL == m_pLoader->GetAnsiString(nRawImportFunctionFOA + sizeof(uint16_t), nNameBufferSize)) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nRawImportFunctionSize = (LibPEAddressT(T))(sizeof(uint16_t) + nNameBufferSize);

    LibPEPtr<PEImportFunctionT<T>> pFunction = new PEImportFunctionT<T>();
    if(NULL == pFunction) {
        return ERR_NO_MEM;
    }

    pFunction->SetParser(this);
    pFunction->SetPEFile(m_pFile);
    pFunction->SetRVA(nRawImportFunctionRVA);
    pFunction->SetSizeInMemory(nRawImportFunctionSize);
    pFunction->SetFOA(nRawImportFunctionFOA);
    pFunction->SetSizeInFile(nRawImportFunctionSize);
    pFunction->SetRawThunkData(pThunkData);

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

    LibPEAddressT(T) nRelocationTableRVA = 0, nRelocationTableFOA = 0, nRelocationTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntryInfo(IMAGE_DIRECTORY_ENTRY_BASERELOC, nRelocationTableRVA, nRelocationTableFOA, nRelocationTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PERelocationTableT<T>> pRelocationTable = new PERelocationTableT<T>();
    if(NULL == pRelocationTable) {
        return ERR_NO_MEM;
    }

    pRelocationTable->SetParser(this);
    pRelocationTable->SetPEFile(m_pFile);
    pRelocationTable->SetRVA(nRelocationTableRVA);
    pRelocationTable->SetSizeInMemory(nRelocationTableSize);
    pRelocationTable->SetFOA(nRelocationTableFOA);
    pRelocationTable->SetSizeInFile(nRelocationTableSize);

    LibPERawBaseRelocation(T) *pRawRelocationPage = pRelocationTable->GetRawStruct();
    if(NULL == pRawRelocationPage) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nRelocationPageRVA = nRelocationTableRVA;
    LibPEAddressT(T) nRelocationPageFOA = nRelocationTableFOA;
    while(0 != pRawRelocationPage->VirtualAddress) {
        uint16_t *pRawItemList = (uint16_t *)(&pRawRelocationPage[1]);
        uint32_t nItemIndex = 0;
        uint32_t nItemCount = (pRawRelocationPage->SizeOfBlock - sizeof(LibPERawBaseRelocation(T))) / sizeof(uint16_t);

        LibPEPtr<PERelocationPageT<T>> pRelocationPage = new PERelocationPageT<T>;
        if(NULL == pRelocationPage) {
            return ERR_NO_MEM;
        }

        pRelocationPage->SetParser(this);
        pRelocationPage->SetPEFile(m_pFile);
        pRelocationPage->SetRVA(nRelocationPageRVA);
        pRelocationPage->SetSizeInMemory(sizeof(LibPERawBaseRelocation(T)) + nItemCount * sizeof(uint16_t));
        pRelocationPage->SetFOA(nRelocationPageFOA);
        pRelocationPage->SetSizeInFile(sizeof(LibPERawBaseRelocation(T)) + nItemCount * sizeof(uint16_t));
        pRelocationTable->InnerAddRelocationPage(pRelocationPage);

        LibPEAddressT(T) nRelocationItemRVA = nRelocationPageRVA + sizeof(LibPERawBaseRelocation(T));
        LibPEAddressT(T) nRelocationItemFOA = nRelocationPageFOA + sizeof(LibPERawBaseRelocation(T));
        while(nItemIndex < nItemCount) {
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
            pRelocationItem->InnerSetRelocateFlag(pRawItemList[nItemIndex] & 0xF000);
            pRelocationItem->InnerSetAddressRVA(pRawRelocationPage->VirtualAddress + (pRawItemList[nItemIndex] & 0x0FFF));
            pRelocationPage->InnerAddRelocationItem(pRelocationItem);

            nRelocationItemRVA += sizeof(uint16_t);
            nRelocationItemFOA += sizeof(uint16_t);
            ++nItemIndex;
        }

        nRelocationPageRVA = nRelocationItemRVA;
        nRelocationPageFOA = nRelocationItemFOA;
        pRawRelocationPage = (LibPERawBaseRelocation(T) *)(((uint8_t *)pRawRelocationPage) + sizeof(LibPERawBaseRelocation(T)) + sizeof(uint16_t) * nItemCount);
    }

    *ppRelocationTable = pRelocationTable.Detach();

    return ERR_OK;
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
    LIBPE_ASSERT_RET(NULL != ppImportAddressTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppImportAddressTable = NULL;

    LibPEAddressT(T) nImportAddressTableRVA = 0, nImportAddressTableFOA = 0, nImportAddressTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntryInfo(IMAGE_DIRECTORY_ENTRY_IAT, nImportAddressTableRVA, nImportAddressTableFOA, nImportAddressTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEImportAddressTableT<T>> pImportAddressTable = new PEImportAddressTableT<T>();
    if(NULL == pImportAddressTable) {
        return ERR_NO_MEM;
    }

    pImportAddressTable->SetParser(this);
    pImportAddressTable->SetPEFile(m_pFile);
    pImportAddressTable->SetRVA(nImportAddressTableRVA);
    pImportAddressTable->SetSizeInMemory(nImportAddressTableSize);
    pImportAddressTable->SetFOA(nImportAddressTableFOA);
    pImportAddressTable->SetSizeInFile(nImportAddressTableSize);

    if(ERR_OK != ParseImportAddressTableContent(pImportAddressTable->GetRawStruct(), pImportAddressTable)) {
        return ERR_FAIL;
    }

    *ppImportAddressTable = pImportAddressTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportAddressTableContent(LibPERawThunkData(T) *pRawTable, IPEImportAddressTableT<T> *pImportAddressTable)
{
    LIBPE_ASSERT_RET(NULL != pRawTable && NULL != pImportAddressTable, ERR_POINTER);

    PEImportAddressTableT<T> *pInnerImportAddressTable = static_cast<PEImportAddressTableT<T> *>(pImportAddressTable);
    LibPEAddressT(T) nTableRVA = pImportAddressTable->GetRVA();
    LibPEAddressT(T) nTableFOA = pImportAddressTable->GetFOA();

    LibPERawThunkData(T) *pRawBlock = pRawTable;
    LibPEAddressT(T) nParsedSize = 0, nTotalSize = pInnerImportAddressTable->GetSizeInFile();

    while(nParsedSize < nTotalSize && 0 != pRawBlock->u1.Function) {
        LibPEPtr<IPEImportAddressBlockT<T>> pBlock;
        if(ERR_OK != ParseImportAddressBlock(pRawBlock, nTableRVA + nParsedSize, nTableFOA + nParsedSize, &pBlock) || NULL == pBlock) {
            return ERR_FAIL;
        }
        pInnerImportAddressTable->InnerAddImportAddressBlock(pBlock);

        nParsedSize += pBlock->GetSizeInFile();
        pRawBlock = (LibPERawThunkData(T) *)(((uint8_t *)pRawTable) + nParsedSize);
    }

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, LibPEAddressT(T) nBlockRVA, LibPEAddressT(T) nBlockFOA, IPEImportAddressBlockT<T> **ppBlock)
{
    LIBPE_ASSERT_RET(0 != nBlockRVA && 0 != nBlockFOA, ERR_INVALID_ARG);
    LIBPE_ASSERT_RET(NULL != ppBlock, ERR_POINTER);

    *ppBlock = NULL;

    LibPEPtr<PEImportAddressBlockT<T>> pBlock = new PEImportAddressBlockT<T>();
    if(NULL == pBlock) {
        return ERR_NO_MEM;
    }

    pBlock->SetParser(this);
    pBlock->SetPEFile(m_pFile);
    pBlock->SetRawMemory(pRawBlock);
    pBlock->SetRVA(nBlockRVA);
    pBlock->SetFOA(nBlockFOA);
    
    // Reload FOA to ensure it is avaliable. Because we need it right now.
    nBlockFOA = pBlock->GetFOA();

    // If the RawBlock is NULL, we should ready the memory ourself.
    LibPERawThunkData(T) *pRawItem = NULL;
    bool_t bNeedLoadMemory = false;
    if(NULL == pRawBlock) {
        LIBPE_ASSERT_RET(NULL != m_pLoader, ERR_FAIL);
        pRawItem = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nBlockFOA, sizeof(LibPERawThunkData(T)));
        LIBPE_ASSERT_RET(NULL != pRawBlock, ERR_NO_MEM);
        bNeedLoadMemory = true;
    } else {
        pRawItem = pRawBlock;
    }

    // Parse the child import address item. If you don't do this, we will never know the size of this block.
    LibPEAddressT(T) nBlockSize = 0;
    while(pRawItem->u1.Function != 0) {
        LibPEPtr<IPEImportAddressItemT<T>> pItem;
        if(ERR_OK != ParseImportAddressItem(pRawItem, nBlockRVA + nBlockSize, nBlockFOA + nBlockSize, &pItem)) {
            return ERR_FAIL;
        }
        pBlock->InnerAddImportAddressItem(pItem);

        nBlockSize += sizeof(LibPERawThunkData(T));

        if(bNeedLoadMemory) {
            pRawItem = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nBlockFOA + nBlockSize, sizeof(LibPERawThunkData(T)));
            LIBPE_ASSERT_RET(NULL != pRawItem, ERR_NO_MEM);
        } else {
            ++pRawItem;
        }
    }

    nBlockSize += sizeof(LibPERawThunkData(T));

    pBlock->SetSizeInMemory(nBlockSize);
    pBlock->SetSizeInFile(nBlockSize);

    *ppBlock = pBlock.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, LibPEAddressT(T) nItemRVA, LibPEAddressT(T) nItemFOA, IPEImportAddressItemT<T> **ppItem)
{
    // We can ignore the condition - (NULL != pRawItem) here.
    // Because the memory will be ready while you call GetRawStruct() at the first time.
    LIBPE_ASSERT_RET(NULL != ppItem, ERR_POINTER);
    LIBPE_ASSERT_RET(0 != nItemRVA && 0 != nItemFOA, ERR_INVALID_ARG);

    *ppItem = NULL;

    LibPEPtr<PEImportAddressItemT<T>> pItem = new PEImportAddressItemT<T>();
    if(NULL == pItem) {
        return ERR_NO_MEM;
    }

    pItem->SetParser(this);
    pItem->SetPEFile(m_pFile);
    pItem->SetRawMemory(pRawItem);
    pItem->SetRVA(nItemRVA);
    pItem->SetSizeInMemory(sizeof(LibPERawThunkData(T)));
    pItem->SetFOA(nItemFOA);
    pItem->SetSizeInFile(sizeof(LibPERawThunkData(T)));

    *ppItem = pItem.Detach();

    return ERR_OK;
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