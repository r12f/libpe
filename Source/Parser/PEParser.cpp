#include "stdafx.h"
#include "Parser/PEParser.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PESectionHeader.h"
#include "PE/PESection.h"
#include "PE/PEExportTable.h"
#include "PE/PEImportTable.h"
#include "PE/PEResourceTable.h"
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
PEParserT<T> *
PEParserT<T>::CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile)
{
    LIBPE_ASSERT_RET(NULL != pFilePath && NULL != pFile, NULL);

    LibPEPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.p;
    LIBPE_ASSERT_RET(pRawDataLoader->LoadFile(pFilePath), NULL);

    PEParserT<T > *pParser = PEParserT::Create(PE_PARSER_TYPE_DISK_FILE);
    LIBPE_ASSERT_RET(NULL != pParser, NULL);

    PEParserDiskFileT<T > *pRawParser = (PEParserDiskFileT<T > *)pParser;

    pRawParser->SetPEFile(pFile);
    pRawParser->SetDataLoader(pDataLoader);

    return pParser;
}

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForMappedFile(void *pMemory, PEFileT<T> *pFile)
{
    return NULL;
}

#ifdef LIBPE_WINOS

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

#endif

template <class T>
void *
PEParserT<T>::GetRawMemory(uint64_t nOffset, uint64_t nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader, NULL);
    return m_pLoader->GetBuffer(nOffset, nSize);
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetRVAFromVA(LibPEAddressT(T) nVA)
{
    LIBPE_ASSERT_RET(NULL != m_pFile, 0);
    LibPERawOptionalHeaderT(T) *pOptinalHeader = m_pFile->GetOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, NULL);
    if(nVA < pOptinalHeader->ImageBase) {
        return 0;
    }
    return nVA - pOptinalHeader->ImageBase;
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetVAFromRVA(LibPEAddressT(T) nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pFile, 0);
    LibPERawOptionalHeaderT(T) *pOptinalHeader = m_pFile->GetOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, NULL);
    return pOptinalHeader->ImageBase + nRVA;
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetRVAFromFOA(LibPEAddressT(T) nFOA)
{
    LIBPE_ASSERT_RET(NULL != m_pFile, 0);
    
    LibPEPtr<IPESectionT<T>> pLastSection, pSection;
    uint32_t nSectionId = 0, nSectionCount = m_pFile->GetSectionCount();
    for(nSectionId = 0; nSectionId < nSectionCount; ++nSectionId) {
        if(ERR_OK != m_pFile->GetSection(nSectionId, &pSection) || NULL == pSection) {
            return 0;
        }

        if(pSection->GetFOA() > nFOA) {
            break;
        }

        pLastSection.Attach(pSection.Detach());
    }

    if(NULL == pLastSection) {
        return nFOA;
    }

    return pLastSection->GetRVA() + nFOA - pLastSection->GetFOA();
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetFOAFromRVA(LibPEAddressT(T) nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pFile, 0);
    
    LibPEPtr<IPESectionT<T>> pLastSection, pSection;
    uint32_t nSectionId = 0, nSectionCount = m_pFile->GetSectionCount();
    for(nSectionId = 0; nSectionId < nSectionCount; ++nSectionId) {
        if(ERR_OK != m_pFile->GetSection(nSectionId, &pSection) || NULL == pSection) {
            return 0;
        }

        if(pSection->GetRVA() > nRVA) {
            break;
        }

        pLastSection.Attach(pSection.Detach());
    }

    if(NULL == pLastSection) {
        return nRVA;
    }

    return pLastSection->GetFOA() + nRVA - pLastSection->GetRVA();
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetVAFromFOA(LibPEAddressT(T) nFOA)
{
    return GetVAFromRVA(GetRVAFromFOA(nFOA));
}

template <class T>
LibPEAddressT(T)
PEParserT<T>::GetFOAFromVA(LibPEAddressT(T) nVA)
{
    return GetFOAFromRVA(GetRVAFromVA(nVA));
}

template <class T>
const char *
PEParserT<T>::ParseAnsiString(LibPEAddressT(T) nRVA, LibPEAddressT(T) nFOA, uint64_t &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);
    return m_pLoader->GetAnsiString(GetRawOffset(nRVA, nFOA), nSize);
}

template <class T>
error_t
PEParserT<T>::ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, LibPERawNtHeadersT(T) **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlayT<T> **ppOverlay)
{
    LIBPE_ASSERT_RET(NULL != ppDosHeader && NULL != ppNtHeaders && NULL != pSectionHeaders && NULL != ppOverlay, ERR_POINTER);
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
    uint32_t nStartSectionHeaderOffset = pDosHeader->e_lfanew + sizeof(DWORD) + sizeof(LibPERawFileHeaderT(T)) + pNtHeaders->FileHeader.SizeOfOptionalHeader;
    uint32_t nSectionHeaderOffset = nStartSectionHeaderOffset;
    LibPEPtr<PESectionHeaderT<T>> pSectionHeader;
    for(uint16_t nSectionId = 0; nSectionId < pNtHeaders->FileHeader.NumberOfSections; ++nSectionId) {
        nSectionHeaderOffset = nStartSectionHeaderOffset + nSectionId * sizeof(LibPERawSectionHeaderT(T));

        pSectionHeader = new PESectionHeaderT<T>();
        if(NULL == pSectionHeader) {
            return ERR_NO_MEM;
        }

        pSectionHeader->InnerSetBase(m_pFile, this);
        pSectionHeader->InnerSetMemoryInfo(nSectionHeaderOffset, 0, sizeof(LibPERawOptionalHeaderT(T)));
        pSectionHeader->InnerSetFileInfo(nSectionHeaderOffset, sizeof(LibPERawOptionalHeaderT(T)));

        pSectionHeaders->push_back(pSectionHeader.p);
    }

    // Parse extra data
    LibPEAddressT(T) nOverlayBeginFOA = nStartSectionHeaderOffset;
    LibPEAddressT(T) nOverlayBeginRVA = nStartSectionHeaderOffset;
    if(NULL != pSectionHeader) {
        LibPEPtr<IPESectionT<T>> pSection;
        if(ERR_OK != pSectionHeader->GetSection(&pSection) || NULL == pSection) {
            return ERR_FAIL;
        }

        nOverlayBeginFOA = pSection->GetFOA() + pSection->GetSizeInFile();
        nOverlayBeginRVA = pSection->GetRVA() + pSection->GetSizeInMemory();
    }

    LibPEAddressT(T) nFileSize = (LibPEAddressT(T))(m_pLoader->GetSize());
    if(nOverlayBeginFOA < nFileSize) {
        LibPEAddressT(T) nOverlaySize = nFileSize - nOverlayBeginFOA;

        LibPEPtr<PEOverlayT<T>> pOverlay = new PEOverlayT<T>();
        if(NULL == pOverlay) {
            return ERR_NO_MEM;
        }
        
        pOverlay->InnerSetBase(m_pFile, this);
        pOverlay->InnerSetMemoryInfo(nOverlayBeginRVA, 0, nOverlaySize);
        pOverlay->InnerSetFileInfo(nOverlayBeginFOA, nOverlaySize);

        *ppOverlay = pOverlay.Detach();
    }

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection)
{
    LIBPE_ASSERT_RET(NULL != pSectionHeader && NULL != ppSection, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    LibPEPtr<PESectionT<T>> pRawSection = new PESectionT<T>();
    if(NULL == pRawSection) {
        return ERR_NO_MEM;
    }

    pRawSection->InnerSetBase(m_pFile, this);
    pRawSection->InnerSetMemoryInfo(pSectionHeader->VirtualAddress, 0, pSectionHeader->SizeOfRawData);
    pRawSection->InnerSetFileInfo(pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData);
    pRawSection->InnerSetSectionHeader(pSectionHeader);

    *ppSection = pRawSection.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseExportTable(IPEExportTableT<T> **ppExportTable)
{
    LIBPE_ASSERT_RET(NULL != ppExportTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppExportTable = NULL;

    LibPEAddressT(T) nExportTableRVA = 0, nExportTableFOA = 0, nExportTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_EXPORT, nExportTableRVA, nExportTableFOA, nExportTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEExportTableT<T>> pExportTable = new PEExportTableT<T>();
    if(NULL == pExportTable) {
        return ERR_NO_MEM;
    }

    pExportTable->InnerSetBase(m_pFile, this);
    pExportTable->InnerSetMemoryInfo(nExportTableRVA, 0, nExportTableSize);
    pExportTable->InnerSetFileInfo(nExportTableFOA, nExportTableSize);

    LibPERawExportDirectory(T) *pExportDirectory = pExportTable->GetRawStruct();
    if(NULL == pExportDirectory) {
        return ERR_NO_MEM;
    }

    LibPEAddressT(T) nFunctionListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfFunctions);
    LibPEAddressT(T) nNameListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfNames);
    LibPEAddressT(T) nNameOrdinalListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfNameOrdinals);

    uint32_t *pFunctionList = (uint32_t *)m_pLoader->GetBuffer(nFunctionListOffset, pExportDirectory->NumberOfFunctions * sizeof(LibPEAddressT(T)));
    uint32_t *pNameList = (uint32_t *)m_pLoader->GetBuffer(nNameListOffset, pExportDirectory->NumberOfNames * sizeof(LibPEAddressT(T)));
    uint16_t *pNameOrdinalList = (uint16_t *)m_pLoader->GetBuffer(nNameOrdinalListOffset, pExportDirectory->NumberOfFunctions * sizeof(uint16_t));

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, ERR_NO_MEM);

    pExportTable->InnerSetFunctionList(pFunctionList);
    pExportTable->InnerSetNameList(pNameList);
    pExportTable->InnerSetNameOrdinalList(pNameOrdinalList);
    
    if(!pExportTable->PrepareForUsing()) {
        return ERR_FAIL;
    }

    *ppExportTable = pExportTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseExportFunction(IPEExportTableT<T> *pExportTable, uint32_t nIndex, IPEExportFunctionT<T> **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != pExportTable && NULL != ppFunction, ERR_POINTER);

    PEExportTableT<T> *pRawExportTable = static_cast<PEExportTableT<T> *>(pExportTable);
    uint32_t *pFunctionList = pRawExportTable->GetRawFunctionList();
    uint32_t *pNameList = pRawExportTable->GetRawNameList();
    uint16_t *pNameOrdinalList = pRawExportTable->GetRawNameOrdinalList();

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, ERR_FAIL);

    LibPEAddressT(T) nFunctionRVA = pFunctionList[nIndex];
    LibPEAddressT(T) nNameRVA = (nIndex < pRawExportTable->GetFunctionCount()) ? pNameList[nIndex] : 0;
    uint16_t nNameOrdinal = pNameOrdinalList[nIndex];

    LibPEPtr<PEExportFunctionT<T>> pFunction = new PEExportFunctionT<T>();
    if(NULL == pFunction) {
        return ERR_NO_MEM;
    }

    pFunction->InnerSetBase(m_pFile, this);
    pFunction->InnerSetMemoryInfo(nFunctionRVA, 0, 0);
    pFunction->InnerSetFileInfo(0, 0);
    pFunction->InnerSetHint(nNameOrdinal);

    if(nNameRVA != 0) {
        LibPEAddressT(T) nNameFOA = GetFOAFromRVA(nNameRVA);
        uint64_t nNameBufferSize = 0;
        const char *pName = m_pLoader->GetAnsiString(nNameFOA, nNameBufferSize);
        pFunction->InnerSetName(pName);
    }

    *ppFunction = pFunction.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseImportTable(IPEImportTableT<T> **ppImportTable)
{
    LIBPE_ASSERT_RET(NULL != ppImportTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppImportTable = NULL;

    LibPEAddressT(T) nImportTableRVA = 0, nImportTableFOA = 0, nImportTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IMPORT, nImportTableRVA, nImportTableFOA, nImportTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEImportTableT<T>> pImportTable = new PEImportTableT<T>();
    if(NULL == pImportTable) {
        return ERR_NO_MEM;
    }

    pImportTable->InnerSetBase(m_pFile, this);
    pImportTable->InnerSetMemoryInfo(nImportTableRVA, 0, nImportTableSize);
    pImportTable->InnerSetFileInfo(nImportTableFOA, nImportTableSize);

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
PEParserT<T>::ParseImportModule(LibPEAddressT(T) nImportDescRVA, LibPEAddressT(T) nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModuleT<T> **ppImportModule)
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

    pImportModule->InnerSetBase(m_pFile, this);
    pImportModule->InnerSetMemoryInfo(nImportDescRVA, 0, sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->InnerSetFileInfo(nImportDescFOA, sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->InnerSetName(pImportName);

    // By default, we use the first bridge to IMAGE_IMPORT_BY_NAME. But in some cases, the first bridge is NULL.
    // Compilers use the second bridge only. So we should fix the thunk entry at that time.
    LibPEAddressT(T) nImportThunkRVA = pImportDescriptor->OriginalFirstThunk;
    if(0 == pImportDescriptor->OriginalFirstThunk) {
        nImportThunkRVA = pImportDescriptor->FirstThunk;
    }
    LIBPE_ASSERT_RET(0 != nImportThunkRVA, ERR_FAIL);

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
PEParserT<T>::ParseImportFunction(LibPERawImportDescriptor(T) *pImportDescriptor, LibPERawThunkData(T) *pThunkData, IPEImportFunctionT<T> **ppFunction)
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

    pFunction->InnerSetBase(m_pFile, this);
    pFunction->InnerSetMemoryInfo(nRawImportFunctionRVA, 0, nRawImportFunctionSize);
    pFunction->InnerSetFileInfo(nRawImportFunctionFOA, nRawImportFunctionSize);
    pFunction->InnerSetThunkData(pThunkData);

    *ppFunction = pFunction.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseResourceTable(IPEResourceTableT<T> **ppResourceTable)
{
    LIBPE_ASSERT_RET(NULL != ppResourceTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppResourceTable = NULL;

    LibPEAddressT(T) nResourceTableRVA = 0, nResourceTableFOA = 0, nResourceTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_RESOURCE, nResourceTableRVA, nResourceTableFOA, nResourceTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEResourceTableT<T>> pInnerTable = new PEResourceTableT<T>();
    if(NULL == pInnerTable) {
        return ERR_NO_MEM;
    }

    pInnerTable->InnerSetBase(m_pFile, this);
    pInnerTable->InnerSetMemoryInfo(nResourceTableRVA, 0, nResourceTableSize);
    pInnerTable->InnerSetFileInfo(nResourceTableFOA, nResourceTableSize);

    LibPEPtr<IPEResourceDirectoryT<T>> pRootDirectory;
    if(ERR_OK != ParseResourceDirectory(nResourceTableRVA, nResourceTableFOA, &pRootDirectory) || NULL == pRootDirectory) {
        return ERR_NO_MEM;
    }

    pInnerTable->InnerSetRootDirectory(pRootDirectory.p);

    *ppResourceTable = pInnerTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseResourceDirectory(LibPEAddressT(T) nRVA, LibPEAddressT(T) nFOA, IPEResourceDirectoryT<T> **ppDirectory)
{
    LIBPE_ASSERT_RET(NULL != ppDirectory, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppDirectory = NULL;

    LibPEPtr<PEResourceDirectoryT<T>> pInnerDirectory = new PEResourceDirectoryT<T>();
    if(NULL == pInnerDirectory) {
        return ERR_NO_MEM;
    }

    pInnerDirectory->InnerSetBase(m_pFile, this);
    pInnerDirectory->InnerSetMemoryInfo(nRVA, 0, sizeof(LibPERawResourceDirectory(T)));
    pInnerDirectory->InnerSetFileInfo(nFOA, sizeof(LibPERawResourceDirectory(T)));

    LibPERawResourceDirectory(T) *pRawDirectory = pInnerDirectory->GetRawStruct();
    if(NULL == pRawDirectory) {
        return ERR_NO_MEM;
    }

    pInnerDirectory->InnerReserveEntry(pRawDirectory->NumberOfNamedEntries + pRawDirectory->NumberOfIdEntries);

    *ppDirectory = pInnerDirectory.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseResourceDirectoryEntry(IPEResourceDirectoryT<T> *pDirectory, uint32_t nEntryIndex, IPEResourceDirectoryEntryT<T> **ppEntry)
{
    LIBPE_ASSERT_RET(NULL != pDirectory && NULL != ppEntry, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppEntry = NULL;

    LibPEAddressT(T) nFirstEntryRVA = pDirectory->GetRVA() + pDirectory->GetSizeInMemory();
    LibPEAddressT(T) nFirstEntryFOA = pDirectory->GetFOA() + pDirectory->GetSizeInFile();

    LibPEAddressT(T) nEntryRVA = nFirstEntryRVA + nEntryIndex * sizeof(LibPERawResourceDirectoryEntry(T));
    LibPEAddressT(T) nEntryFOA = nFirstEntryFOA + nEntryIndex * sizeof(LibPERawResourceDirectoryEntry(T));

    LibPEPtr<PEResourceDirectoryEntryT<T>> pInnerEntry = new PEResourceDirectoryEntryT<T>();
    if(NULL == pInnerEntry) {
        return ERR_NO_MEM;
    }

    pInnerEntry->InnerSetBase(m_pFile, this);
    pInnerEntry->InnerSetMemoryInfo(nEntryRVA, 0, sizeof(LibPERawResourceDirectoryEntry(T)));
    pInnerEntry->InnerSetFileInfo(nEntryFOA, sizeof(LibPERawResourceDirectoryEntry(T)));

    *ppEntry = pInnerEntry.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseResourceDataEntry(LibPEAddressT(T) nRVA, LibPEAddressT(T) nFOA, IPEResourceDataEntryT<T> **ppDataEntry)
{
    LIBPE_ASSERT_RET(NULL != ppDataEntry, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppDataEntry = NULL;

    LibPEPtr<PEResourceDataEntryT<T>> pInnerDataEntry = new PEResourceDataEntryT<T>();
    if(NULL == pInnerDataEntry) {
        return ERR_NO_MEM;
    }

    pInnerDataEntry->InnerSetBase(m_pFile, this);
    pInnerDataEntry->InnerSetMemoryInfo(nRVA, 0, sizeof(LibPERawResourceDataEntry(T)));
    pInnerDataEntry->InnerSetFileInfo(nFOA, sizeof(LibPERawResourceDataEntry(T)));

    LibPERawResourceDataEntry(T) *pRawDataEntry = pInnerDataEntry->GetRawStruct();
    if(NULL == pRawDataEntry) {
        return ERR_NO_MEM;
    }

    *ppDataEntry = pInnerDataEntry.Detach();

    return ERR_OK;
}

template <class T>
LibPERawResourceString(T) *
PEParserT<T>::ParseResourceString(LibPEAddressT(T) nRVA, LibPEAddressT(T) nFOA, uint64_t &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    LibPEAddressT(T) nRawOffset = GetRawOffset(nRVA, nFOA);
    uint16_t *pRawStringSize = (uint16_t *)m_pLoader->GetBuffer(nRawOffset, sizeof(uint16_t));
    if(NULL == pRawStringSize) {
        return NULL;
    }

    if(NULL == m_pLoader->GetBuffer(nRawOffset + sizeof(uint16_t), *pRawStringSize)) {
        return NULL;
    }

    return (LibPERawResourceString(T) *)pRawStringSize;
}

template <class T>
LibPERawResourceStringU(T) *
PEParserT<T>::ParseResourceStringU(LibPEAddressT(T) nRVA, LibPEAddressT(T) nFOA, uint64_t &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    LibPEAddressT(T) nRawOffset = GetRawOffset(nRVA, nFOA);
    uint16_t *pRawStringSize = (uint16_t *)m_pLoader->GetBuffer(nRawOffset, sizeof(uint16_t));
    if(NULL == pRawStringSize) {
        return NULL;
    }

    if(NULL == m_pLoader->GetBuffer(nRawOffset + sizeof(uint16_t), *pRawStringSize)) {
        return NULL;
    }

    return (LibPERawResourceStringU(T) *)pRawStringSize;
}

template <class T>
error_t
PEParserT<T>::ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppRelocationTable = NULL;

    LibPEAddressT(T) nRelocationTableRVA = 0, nRelocationTableFOA = 0, nRelocationTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_BASERELOC, nRelocationTableRVA, nRelocationTableFOA, nRelocationTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PERelocationTableT<T>> pRelocationTable = new PERelocationTableT<T>();
    if(NULL == pRelocationTable) {
        return ERR_NO_MEM;
    }

    pRelocationTable->InnerSetBase(m_pFile, this);
    pRelocationTable->InnerSetMemoryInfo(nRelocationTableRVA, 0, nRelocationTableSize);
    pRelocationTable->InnerSetFileInfo(nRelocationTableFOA, nRelocationTableSize);

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

        LibPEAddressT(T) nPageSize = sizeof(LibPERawBaseRelocation(T)) + nItemCount * sizeof(uint16_t);

        pRelocationPage->InnerSetBase(m_pFile, this);
        pRelocationPage->InnerSetMemoryInfo(nRelocationPageRVA, 0, nPageSize);
        pRelocationPage->InnerSetFileInfo(nRelocationPageFOA, nPageSize);

        pRelocationTable->InnerAddRelocationPage(pRelocationPage);

        LibPEAddressT(T) nRelocationItemRVA = nRelocationPageRVA + sizeof(LibPERawBaseRelocation(T));
        LibPEAddressT(T) nRelocationItemFOA = nRelocationPageFOA + sizeof(LibPERawBaseRelocation(T));
        while(nItemIndex < nItemCount) {
            LibPEPtr<PERelocationItemT<T>> pRelocationItem = new PERelocationItemT<T>();
            if(NULL == pRelocationItem) {
                return ERR_NO_MEM;
            }

            pRelocationItem->InnerSetBase(m_pFile, this);
            pRelocationItem->InnerSetMemoryInfo(nRelocationItemRVA, 0, sizeof(uint16_t));
            pRelocationItem->InnerSetFileInfo(nRelocationItemFOA, sizeof(uint16_t));
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
PEParserT<T>::ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseTlsTable(IPETlsTableT<T> **ppTlsTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable)
{
    LIBPE_ASSERT_RET(NULL != ppImportAddressTable, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    *ppImportAddressTable = NULL;

    LibPEAddressT(T) nImportAddressTableRVA = 0, nImportAddressTableFOA = 0, nImportAddressTableSize = 0;
    if(ERR_OK != GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IAT, nImportAddressTableRVA, nImportAddressTableFOA, nImportAddressTableSize)) {
        return ERR_FAIL;
    }

    LibPEPtr<PEImportAddressTableT<T>> pImportAddressTable = new PEImportAddressTableT<T>();
    if(NULL == pImportAddressTable) {
        return ERR_NO_MEM;
    }

    pImportAddressTable->InnerSetBase(m_pFile, this);
    pImportAddressTable->InnerSetMemoryInfo(nImportAddressTableRVA, 0, nImportAddressTableSize);
    pImportAddressTable->InnerSetFileInfo(nImportAddressTableFOA, nImportAddressTableSize);

    if(ERR_OK != ParseImportAddressTableContent(pImportAddressTable)) {
        return ERR_FAIL;
    }

    *ppImportAddressTable = pImportAddressTable.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseImportAddressTableContent(IPEImportAddressTableT<T> *pImportAddressTable)
{
    LIBPE_ASSERT_RET(NULL != pImportAddressTable, ERR_POINTER);

    LibPERawThunkData(T) *pRawTable = pImportAddressTable->GetRawStruct();
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
PEParserT<T>::ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, LibPEAddressT(T) nBlockRVA, LibPEAddressT(T) nBlockFOA, IPEImportAddressBlockT<T> **ppBlock)
{
    LIBPE_ASSERT_RET(0 != nBlockRVA, ERR_INVALID_ARG);
    LIBPE_ASSERT_RET(NULL != ppBlock, ERR_POINTER);

    *ppBlock = NULL;

    LibPEPtr<PEImportAddressBlockT<T>> pBlock = new PEImportAddressBlockT<T>();
    if(NULL == pBlock) {
        return ERR_NO_MEM;
    }

    pBlock->InnerSetBase(m_pFile, this);
    pBlock->InnerSetRawMemory(pRawBlock);
    pBlock->InnerSetMemoryInfo(nBlockRVA, 0, 0);
    pBlock->InnerSetFileInfo(nBlockFOA, 0);

    // Reload FOA to ensure it is avaliable. Because we need it right now.
    if(0 == nBlockFOA) {
        nBlockFOA = GetFOAFromRVA(nBlockRVA);
    }

    // If the RawBlock is NULL, we should ready the memory ourself.
    LibPERawThunkData(T) *pRawItem = NULL;
    bool_t bNeedLoadMemory = false;
    if(NULL == pRawBlock) {
        LIBPE_ASSERT_RET(NULL != m_pLoader, ERR_FAIL);
        pRawItem = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nBlockFOA, sizeof(LibPERawThunkData(T)));
        LIBPE_ASSERT_RET(NULL != pRawItem, ERR_NO_MEM);
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

    pBlock->InnerSetMemoryInfo(nBlockRVA, 0, nBlockSize);
    pBlock->InnerSetFileInfo(nBlockFOA, nBlockSize);

    *ppBlock = pBlock.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, LibPEAddressT(T) nItemRVA, LibPEAddressT(T) nItemFOA, IPEImportAddressItemT<T> **ppItem)
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

    pItem->InnerSetBase(m_pFile, this);
    pItem->InnerSetRawMemory(pRawItem);
    pItem->InnerSetMemoryInfo(nItemRVA, 0, sizeof(LibPERawThunkData(T)));
    pItem->InnerSetFileInfo(nItemRVA, sizeof(LibPERawThunkData(T)));

    *ppItem = pItem.Detach();

    return ERR_OK;
}

template <class T>
error_t
PEParserT<T>::ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEParserT<T>::ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader)
{
    return ERR_NOT_IMPL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, Create);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, CreateForDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, CreateForMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, CreateForMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, CreateForLoadedModule);
#endif

LIBPE_NAMESPACE_END