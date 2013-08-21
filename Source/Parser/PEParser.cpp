#include "stdafx.h"
#include "Parser/PEParser.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PEHeader.h"
#include "PE/PESection.h"
#include "PE/PEExportTable.h"
#include "PE/PEImportTable.h"
#include "PE/PEResourceTable.h"
#include "PE/PERelocationTable.h"
#include "PE/PEImportAddressTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
LibPEPtr<PEParserT<T>>
PEParserT<T>::Create(PEParserType nType)
{
    switch(nType) {
    case PE_PARSER_TYPE_DISK_FILE:
        return new PEParserDiskFileT<T>;
    }

    return NULL;
}

template <class T>
PEAddress
PEParserT<T>::GetRVAFromVA(PEAddress nVA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nVA, LIBPE_INVALID_ADDRESS);
    LIBPE_ASSERT_RET(NULL != m_pFile, LIBPE_INVALID_ADDRESS);

    LibPERawOptionalHeaderT(T) *pOptinalHeader = (LibPERawOptionalHeaderT(T) *)m_pFile->GetRawOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, LIBPE_INVALID_ADDRESS);
    if(nVA < pOptinalHeader->ImageBase) {
        return LIBPE_INVALID_ADDRESS;
    }

    return nVA - pOptinalHeader->ImageBase;
}

template <class T>
PEAddress
PEParserT<T>::GetVAFromRVA(PEAddress nRVA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRVA, LIBPE_INVALID_ADDRESS);
    LIBPE_ASSERT_RET(NULL != m_pFile, LIBPE_INVALID_ADDRESS);

    LibPERawOptionalHeaderT(T) *pOptinalHeader = (LibPERawOptionalHeaderT(T) *)m_pFile->GetRawOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, LIBPE_INVALID_ADDRESS);

    return pOptinalHeader->ImageBase + nRVA;
}

template <class T>
PEAddress
PEParserT<T>::GetRVAFromFOA(PEAddress nFOA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nFOA, LIBPE_INVALID_ADDRESS);
    LIBPE_ASSERT_RET(NULL != m_pFile, LIBPE_INVALID_ADDRESS);
    
    LibPEPtr<IPESection> pLastSection, pSection;
    UINT32 nSectionId = 0, nSectionCount = m_pFile->GetSectionCount();
    for(nSectionId = 0; nSectionId < nSectionCount; ++nSectionId) {
        if(FAILED(m_pFile->GetSection(nSectionId, &pSection)) || NULL == pSection) {
            return LIBPE_INVALID_ADDRESS;
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
PEAddress
PEParserT<T>::GetFOAFromRVA(PEAddress nRVA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRVA, LIBPE_INVALID_ADDRESS);
    LIBPE_ASSERT_RET(NULL != m_pFile, LIBPE_INVALID_ADDRESS);
    
    LibPEPtr<IPESection> pLastSection, pSection;
    UINT32 nSectionId = 0, nSectionCount = m_pFile->GetSectionCount();
    for(nSectionId = 0; nSectionId < nSectionCount; ++nSectionId) {
        if(FAILED(m_pFile->GetSection(nSectionId, &pSection)) || NULL == pSection) {
            return LIBPE_INVALID_ADDRESS;
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
PEAddress
PEParserT<T>::GetVAFromFOA(PEAddress nFOA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nFOA, LIBPE_INVALID_ADDRESS);
    return GetVAFromRVA(GetRVAFromFOA(nFOA));
}

template <class T>
PEAddress
PEParserT<T>::GetFOAFromVA(PEAddress nVA)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nVA, LIBPE_INVALID_ADDRESS);
    return GetFOAFromRVA(GetRVAFromVA(nVA));
}

template <class T>
void *
PEParserT<T>::GetRawMemory(UINT64 nOffset, UINT64 nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader, NULL);
    return m_pLoader->GetBuffer(nOffset, nSize);
}

template <class T>
const char *
PEParserT<T>::ParseAnsiString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    PEAddress nOffset = GetRawOffset(nRVA, nFOA);
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nOffset, NULL);

    return m_pLoader->GetAnsiString(nOffset, nSize);
}

template <class T>
const wchar_t *
PEParserT<T>::ParseUnicodeString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    PEAddress nOffset = GetRawOffset(nRVA, nFOA);
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nOffset, NULL);

    return m_pLoader->GetUnicodeString(nOffset, nSize);
}

template <class T>
HRESULT
PEParserT<T>::ParseBasicInfo(IPEDosHeader **ppDosHeader, IPENtHeaders **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlay **ppOverlay)
{
    LIBPE_ASSERT_RET(NULL != ppDosHeader && NULL != ppNtHeaders && NULL != pSectionHeaders && NULL != ppOverlay, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    // Parse dos header
    LibPEPtr<PEDosHeaderT<T>> pInnerDosHeader = new PEDosHeaderT<T>();
    LIBPE_ASSERT_RET(NULL != pInnerDosHeader, E_OUTOFMEMORY);
    pInnerDosHeader->InnerSetBase(m_pFile, this);
    pInnerDosHeader->InnerSetMemoryInfo(0, LIBPE_INVALID_ADDRESS, sizeof(LibPERawDosHeaderT(T)));
    pInnerDosHeader->InnerSetFileInfo(0, sizeof(LibPERawDosHeaderT(T)));

    LibPERawDosHeaderT(T) *pRawDosHeader = pInnerDosHeader->GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pRawDosHeader, E_OUTOFMEMORY);
    LIBPE_ASSERT_RET(IMAGE_DOS_SIGNATURE == pRawDosHeader->e_magic, E_FAIL);

    // Parse nt headers
    LibPEPtr<PENtHeadersT<T>> pInnerNtHeaders = new PENtHeadersT<T>();
    LIBPE_ASSERT_RET(NULL != pInnerNtHeaders, E_OUTOFMEMORY);
    pInnerNtHeaders->InnerSetBase(m_pFile, this);
    pInnerNtHeaders->InnerSetMemoryInfo(pRawDosHeader->e_lfanew, LIBPE_INVALID_ADDRESS, sizeof(LibPERawNtHeadersT(T)));
    pInnerNtHeaders->InnerSetFileInfo(pRawDosHeader->e_lfanew, sizeof(LibPERawNtHeadersT(T)));

    LibPERawNtHeadersT(T) *pRawNtHeaders = pInnerNtHeaders->GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pRawNtHeaders, E_OUTOFMEMORY);
    LIBPE_ASSERT_RET(IMAGE_NT_SIGNATURE == pRawNtHeaders->Signature, E_FAIL);

    if(PETrait<T>::Is32Bit) {
        if(pRawNtHeaders->FileHeader.SizeOfOptionalHeader != sizeof(PERawOptionalHeader32)) {
            return E_FAIL;
        }
    }

    // Parse file header
    LibPEPtr<PEFileHeaderT<T>> pInnerFileHeader = new PEFileHeaderT<T>();
    LIBPE_ASSERT_RET(NULL != pInnerFileHeader, E_OUTOFMEMORY);
    pInnerFileHeader->InnerSetBase(m_pFile, this);
    pInnerFileHeader->InnerSetMemoryInfo(pRawDosHeader->e_lfanew + sizeof(UINT32), LIBPE_INVALID_ADDRESS, sizeof(LibPERawFileHeaderT(T)));
    pInnerFileHeader->InnerSetFileInfo(pRawDosHeader->e_lfanew + sizeof(UINT32), sizeof(LibPERawFileHeaderT(T)));
    pInnerNtHeaders->InnerSetFileHeader(pInnerFileHeader.p);

    LibPEPtr<PEOptionalHeaderT<T>> pInnerOptionalHeader = new PEOptionalHeaderT<T>();
    LIBPE_ASSERT_RET(NULL != pInnerOptionalHeader, E_OUTOFMEMORY);
    pInnerOptionalHeader->InnerSetBase(m_pFile, this);
    pInnerOptionalHeader->InnerSetMemoryInfo(pRawDosHeader->e_lfanew + sizeof(UINT32) + sizeof(LibPERawFileHeaderT(T)), LIBPE_INVALID_ADDRESS, sizeof(LibPERawOptionalHeaderT(T)));
    pInnerOptionalHeader->InnerSetFileInfo(pRawDosHeader->e_lfanew + sizeof(UINT32) + sizeof(LibPERawFileHeaderT(T)), sizeof(LibPERawOptionalHeaderT(T)));
    pInnerNtHeaders->InnerSetOptionalHeader(pInnerOptionalHeader.p);

    *ppDosHeader = pInnerDosHeader.Detach();
    *ppNtHeaders = pInnerNtHeaders.Detach();

    // Parse section headers
    UINT32 nStartSectionHeaderOffset = pRawDosHeader->e_lfanew + sizeof(DWORD) + sizeof(LibPERawFileHeaderT(T)) + pRawNtHeaders->FileHeader.SizeOfOptionalHeader;
    UINT32 nSectionHeaderOffset = nStartSectionHeaderOffset;
    LibPEPtr<PESectionHeaderT<T>> pSectionHeader;
    for(UINT16 nSectionId = 0; nSectionId < pRawNtHeaders->FileHeader.NumberOfSections; ++nSectionId) {
        nSectionHeaderOffset = nStartSectionHeaderOffset + nSectionId * sizeof(LibPERawSectionHeaderT(T));

        pSectionHeader = new PESectionHeaderT<T>();
        if(NULL == pSectionHeader) {
            return E_OUTOFMEMORY;
        }

        pSectionHeader->InnerSetBase(m_pFile, this);
        pSectionHeader->InnerSetMemoryInfo(nSectionHeaderOffset, LIBPE_INVALID_ADDRESS, sizeof(LibPERawOptionalHeaderT(T)));
        pSectionHeader->InnerSetFileInfo(nSectionHeaderOffset, sizeof(LibPERawOptionalHeaderT(T)));

        pSectionHeaders->push_back(pSectionHeader.p);
    }

    // Parse extra data
    PEAddress nOverlayBeginFOA = nStartSectionHeaderOffset;
    PEAddress nOverlayBeginRVA = nStartSectionHeaderOffset;
    if(NULL != pSectionHeader) {
        LibPEPtr<IPESection> pSection;
        if(FAILED(pSectionHeader->GetSection(&pSection)) || NULL == pSection) {
            return E_FAIL;
        }

        nOverlayBeginFOA = pSection->GetFOA() + pSection->GetSizeInFile();
        nOverlayBeginRVA = pSection->GetRVA() + pSection->GetSizeInMemory();
    }

    PEAddress nFileSize = (PEAddress)(m_pLoader->GetSize());
    if(nOverlayBeginFOA < nFileSize) {
        PEAddress nOverlaySize = nFileSize - nOverlayBeginFOA;

        LibPEPtr<PEOverlayT<T>> pOverlay = new PEOverlayT<T>();
        if(NULL == pOverlay) {
            return E_OUTOFMEMORY;
        }
        
        pOverlay->InnerSetBase(m_pFile, this);
        pOverlay->InnerSetMemoryInfo(nOverlayBeginRVA, LIBPE_INVALID_ADDRESS, nOverlaySize);
        pOverlay->InnerSetFileInfo(nOverlayBeginFOA, nOverlaySize);

        *ppOverlay = pOverlay.Detach();
    }

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != pSectionHeader, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppSection, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    LibPEPtr<PESectionT<T>> pRawSection = new PESectionT<T>();
    if(NULL == pRawSection) {
        return E_OUTOFMEMORY;
    }

    pRawSection->InnerSetBase(m_pFile, this);
    pRawSection->InnerSetMemoryInfo(pSectionHeader->VirtualAddress, LIBPE_INVALID_ADDRESS, pSectionHeader->SizeOfRawData);
    pRawSection->InnerSetFileInfo(pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData);
    pRawSection->InnerSetSectionHeader(pSectionHeader);

    *ppSection = pRawSection.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseExportTable(IPEExportTable **ppExportTable)
{
    LIBPE_ASSERT_RET(NULL != ppExportTable, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppExportTable = NULL;

    PEAddress nExportTableRVA = LIBPE_INVALID_ADDRESS, nExportTableFOA = LIBPE_INVALID_ADDRESS, nExportTableSize = LIBPE_INVALID_SIZE;
    if(FAILED(GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_EXPORT, nExportTableRVA, nExportTableFOA, nExportTableSize))) {
        return E_FAIL;
    }

    LibPEPtr<PEExportTableT<T>> pExportTable = new PEExportTableT<T>();
    if(NULL == pExportTable) {
        return E_OUTOFMEMORY;
    }

    pExportTable->InnerSetBase(m_pFile, this);
    pExportTable->InnerSetMemoryInfo(nExportTableRVA, LIBPE_INVALID_ADDRESS, nExportTableSize);
    pExportTable->InnerSetFileInfo(nExportTableFOA, nExportTableSize);

    LibPERawExportDirectory(T) *pExportDirectory = pExportTable->GetRawStruct();
    if(NULL == pExportDirectory) {
        return E_OUTOFMEMORY;
    }

    PEAddress nFunctionListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfFunctions);
    PEAddress nNameListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfNames);
    PEAddress nNameOrdinalListOffset = GetRawOffsetFromAddressField(pExportDirectory->AddressOfNameOrdinals);

    UINT32 *pFunctionList = (UINT32 *)m_pLoader->GetBuffer(nFunctionListOffset, pExportDirectory->NumberOfFunctions * sizeof(PEAddress));
    UINT32 *pNameList = (UINT32 *)m_pLoader->GetBuffer(nNameListOffset, pExportDirectory->NumberOfNames * sizeof(PEAddress));
    UINT16 *pNameOrdinalList = (UINT16 *)m_pLoader->GetBuffer(nNameOrdinalListOffset, pExportDirectory->NumberOfFunctions * sizeof(UINT16));

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, E_OUTOFMEMORY);

    pExportTable->InnerSetFunctionList(pFunctionList);
    pExportTable->InnerSetNameList(pNameList);
    pExportTable->InnerSetNameOrdinalList(pNameOrdinalList);
    
    if(!pExportTable->PrepareForUsing()) {
        return E_FAIL;
    }

    *ppExportTable = pExportTable.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseExportFunction(IPEExportTable *pExportTable, UINT32 nIndex, IPEExportFunction **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != pExportTable, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppFunction, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    PEExportTableT<T> *pRawExportTable = static_cast<PEExportTableT<T> *>(pExportTable);
    UINT32 *pFunctionList = pRawExportTable->GetRawFunctionList();
    UINT32 *pNameList = pRawExportTable->GetRawNameList();
    UINT16 *pNameOrdinalList = pRawExportTable->GetRawNameOrdinalList();

    LIBPE_ASSERT_RET(NULL != pFunctionList && NULL != pNameList && NULL != pNameOrdinalList, E_FAIL);

    PEAddress nFunctionRVA = pFunctionList[nIndex];
    PEAddress nNameRVA = (nIndex < pRawExportTable->GetFunctionCount()) ? pNameList[nIndex] : LIBPE_INVALID_ADDRESS;
    UINT16 nNameOrdinal = pNameOrdinalList[nIndex];

    LibPEPtr<PEExportFunctionT<T>> pFunction = new PEExportFunctionT<T>();
    if(NULL == pFunction) {
        return E_OUTOFMEMORY;
    }

    pFunction->InnerSetBase(m_pFile, this);
    pFunction->InnerSetMemoryInfo(nFunctionRVA, LIBPE_INVALID_ADDRESS, 0);
    pFunction->InnerSetFileInfo(0, 0);
    pFunction->InnerSetOrdinal(nNameOrdinal);

    if(nNameRVA != LIBPE_INVALID_ADDRESS) {
        PEAddress nNameFOA = GetFOAFromRVA(nNameRVA);
        UINT64 nNameBufferSize = 0;
        const char *pName = m_pLoader->GetAnsiString(nNameFOA, nNameBufferSize);
        pFunction->InnerSetName(pName);
    }

    *ppFunction = pFunction.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportTable(IPEImportTable **ppImportTable)
{
    LIBPE_ASSERT_RET(NULL != ppImportTable, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppImportTable = NULL;

    PEAddress nImportTableRVA = LIBPE_INVALID_ADDRESS, nImportTableFOA = LIBPE_INVALID_ADDRESS, nImportTableSize = LIBPE_INVALID_SIZE;
    if(FAILED(GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IMPORT, nImportTableRVA, nImportTableFOA, nImportTableSize))) {
        return E_FAIL;
    }

    LibPEPtr<PEImportTableT<T>> pImportTable = new PEImportTableT<T>();
    if(NULL == pImportTable) {
        return E_OUTOFMEMORY;
    }

    pImportTable->InnerSetBase(m_pFile, this);
    pImportTable->InnerSetMemoryInfo(nImportTableRVA, LIBPE_INVALID_ADDRESS, nImportTableSize);
    pImportTable->InnerSetFileInfo(nImportTableFOA, nImportTableSize);

    LibPERawImportDescriptor(T) *pImportDesc = pImportTable->GetRawStruct();
    if(NULL == pImportDesc) {
        return E_OUTOFMEMORY;
    }

    PEAddress nImportDescRVA = nImportTableRVA, nImportDescFOA = nImportTableFOA;
    while(0 != pImportDesc->Characteristics && 0 != pImportDesc->Name) {
        pImportTable->InnerAddImportDescriptor(nImportDescRVA, nImportDescFOA, pImportDesc);
        ++pImportDesc;
        nImportDescRVA += sizeof(LibPERawImportDescriptor(T));
        nImportDescFOA += sizeof(LibPERawImportDescriptor(T));
    }

    *ppImportTable = pImportTable.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportModule(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModule **ppImportModule)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nImportDescRVA && LIBPE_INVALID_ADDRESS != nImportDescFOA && NULL != pImportDescriptor, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppImportModule, E_POINTER);

    *ppImportModule = NULL;

    PEAddress nImportNameFOA = GetFOAFromRVA(pImportDescriptor->Name);
    if(LIBPE_INVALID_ADDRESS == nImportNameFOA) {
        return E_FAIL;
    }

    UINT64 nNameBufferSize = 0;
    const char *pImportName = m_pLoader->GetAnsiString(nImportNameFOA, nNameBufferSize);

    LibPEPtr<PEImportModuleT<T>> pImportModule = new PEImportModuleT<T>();
    LIBPE_ASSERT_RET(NULL != pImportModule, E_OUTOFMEMORY);

    pImportModule->InnerSetBase(m_pFile, this);
    pImportModule->InnerSetMemoryInfo(nImportDescRVA, LIBPE_INVALID_ADDRESS, sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->InnerSetFileInfo(nImportDescFOA, sizeof(IMAGE_IMPORT_BY_NAME));
    pImportModule->InnerSetName(pImportName);

    *ppImportModule = pImportModule.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportFunctionsInModule(IPEImportModule *pImportModule)
{
    LIBPE_ASSERT_RET(NULL != pImportModule, E_UNEXPECTED);

    PEImportModuleT<T> *pInnerImportModule = (PEImportModuleT<T> *)pImportModule;

    LibPERawImportDescriptor(T) *pImportDescriptor = pInnerImportModule->GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportDescriptor, E_FAIL);

    // By default, we use the first bridge to IMAGE_IMPORT_BY_NAME. But in some cases, the first bridge is NULL.
    // Compilers use the second bridge only. So we should fix the thunk entry at that time.
    PEAddress nThunkDataRVA = pImportDescriptor->OriginalFirstThunk;
    if(0 == pImportDescriptor->OriginalFirstThunk) {
        nThunkDataRVA = pImportDescriptor->FirstThunk;
    }
    LIBPE_ASSERT_RET(0 != nThunkDataRVA, E_FAIL);

    PEAddress nThunkDataFOA = GetFOAFromRVA(nThunkDataRVA);
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nThunkDataFOA, E_FAIL);

    for(;;) {
        LibPERawThunkData(T) *pThunkData = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nThunkDataFOA, sizeof(LibPERawThunkData(T)));
        if(NULL == pThunkData || 0 == pThunkData->u1.AddressOfData) {
            break;
        }

        LibPEPtr<PEImportFunctionT<T>> pFunction = new PEImportFunctionT<T>();
        if(NULL == pFunction) {
            return E_OUTOFMEMORY;
        }

        pFunction->InnerSetBase(m_pFile, this);
        pFunction->InnerSetMemoryInfo(nThunkDataRVA, LIBPE_INVALID_ADDRESS, sizeof(LibPERawThunkData(T)));
        pFunction->InnerSetFileInfo(nThunkDataFOA, sizeof(LibPERawThunkData(T)));

        pInnerImportModule->InnerAddImportFunction(pFunction);

        nThunkDataRVA += sizeof(LibPERawThunkData(T));
        nThunkDataFOA += sizeof(LibPERawThunkData(T));
    }

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportFunction(IPEImportFunction *pFunction)
{
    LIBPE_ASSERT_RET(NULL != pFunction, E_UNEXPECTED);

    PEImportFunctionT<T> *pInnerFunction = (PEImportFunctionT<T> *)pFunction;

    LibPERawThunkData(T) *pThunkData = (LibPERawThunkData(T) *)pFunction->GetRawMemory();
    LIBPE_ASSERT_RET(NULL != pThunkData, E_FAIL);

    // No IMAGE_IMPORT_BY_NAME structure if the function is imported by ordinal
    if (0 != (pThunkData->u1.Ordinal & PETrait<T>::ImageOrdinalFlag)) {
        return S_OK;
    }

    // Parse the function imported by IMAGE_IMPORT_BY_NAME
    PEAddress nRawImportByNameRVA = pThunkData->u1.AddressOfData;
    if (0 == nRawImportByNameRVA) {
        return E_FAIL;
    }

    PEAddress nRawImportByNameFOA = GetFOAFromRVA(nRawImportByNameRVA);
    if(LIBPE_INVALID_ADDRESS == nRawImportByNameFOA) {
        return E_FAIL;
    }

    LibPERawImportByName(T) *pImportByName = (LibPERawImportByName(T) *)m_pLoader->GetBuffer(nRawImportByNameFOA, sizeof(LibPERawImportByName(T)));
    if (NULL == pImportByName) {
        return E_OUTOFMEMORY;
    }
    
    UINT64 nNameBufferSize = 0; 
    if(NULL == m_pLoader->GetAnsiString(nRawImportByNameFOA + sizeof(UINT16), nNameBufferSize)) {
        return E_OUTOFMEMORY;
    }

    PEAddress nRawImportByNameSize = (PEAddress)(sizeof(UINT16) + nNameBufferSize);

    pInnerFunction->InnerSetRawImportByName(pImportByName, nRawImportByNameRVA, nRawImportByNameFOA, nRawImportByNameSize);

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseResourceTable(IPEResourceTable **ppResourceTable)
{
    LIBPE_ASSERT_RET(NULL != ppResourceTable, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppResourceTable = NULL;

    PEAddress nResourceTableRVA = LIBPE_INVALID_ADDRESS, nResourceTableFOA = LIBPE_INVALID_ADDRESS, nResourceTableSize = LIBPE_INVALID_SIZE;
    if(FAILED(GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_RESOURCE, nResourceTableRVA, nResourceTableFOA, nResourceTableSize))) {
        return E_FAIL;
    }

    LibPEPtr<PEResourceTableT<T>> pInnerTable = new PEResourceTableT<T>();
    if(NULL == pInnerTable) {
        return E_OUTOFMEMORY;
    }

    pInnerTable->InnerSetBase(m_pFile, this);
    pInnerTable->InnerSetMemoryInfo(nResourceTableRVA, LIBPE_INVALID_ADDRESS, nResourceTableSize);
    pInnerTable->InnerSetFileInfo(nResourceTableFOA, nResourceTableSize);

    LibPEPtr<IPEResourceDirectory> pRootDirectory;
    if(FAILED(ParseResourceDirectory(nResourceTableRVA, nResourceTableFOA, &pRootDirectory)) || NULL == pRootDirectory) {
        return E_OUTOFMEMORY;
    }

    pInnerTable->InnerSetRootDirectory(pRootDirectory.p);

    *ppResourceTable = pInnerTable.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseResourceDirectory(PEAddress nRVA, PEAddress nFOA, IPEResourceDirectory **ppDirectory)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRVA && LIBPE_INVALID_ADDRESS != nFOA, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppDirectory, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppDirectory = NULL;

    LibPEPtr<PEResourceDirectoryT<T>> pInnerDirectory = new PEResourceDirectoryT<T>();
    if(NULL == pInnerDirectory) {
        return E_OUTOFMEMORY;
    }

    pInnerDirectory->InnerSetBase(m_pFile, this);
    pInnerDirectory->InnerSetMemoryInfo(nRVA, LIBPE_INVALID_ADDRESS, sizeof(LibPERawResourceDirectory(T)));
    pInnerDirectory->InnerSetFileInfo(nFOA, sizeof(LibPERawResourceDirectory(T)));

    LibPERawResourceDirectory(T) *pRawDirectory = pInnerDirectory->GetRawStruct();
    if(NULL == pRawDirectory) {
        return E_OUTOFMEMORY;
    }

    pInnerDirectory->InnerReserveEntry(pRawDirectory->NumberOfNamedEntries + pRawDirectory->NumberOfIdEntries);

    *ppDirectory = pInnerDirectory.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseResourceDirectoryEntry(IPEResourceDirectory *pDirectory, UINT32 nEntryIndex, IPEResourceDirectoryEntry **ppEntry)
{
    LIBPE_ASSERT_RET(NULL != pDirectory, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppEntry, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppEntry = NULL;

    PEAddress nFirstEntryRVA = pDirectory->GetRVA() + pDirectory->GetSizeInMemory();
    PEAddress nFirstEntryFOA = pDirectory->GetFOA() + pDirectory->GetSizeInFile();

    PEAddress nEntryRVA = nFirstEntryRVA + nEntryIndex * sizeof(LibPERawResourceDirectoryEntry(T));
    PEAddress nEntryFOA = nFirstEntryFOA + nEntryIndex * sizeof(LibPERawResourceDirectoryEntry(T));

    LibPEPtr<PEResourceDirectoryEntryT<T>> pInnerEntry = new PEResourceDirectoryEntryT<T>();
    if(NULL == pInnerEntry) {
        return E_OUTOFMEMORY;
    }

    pInnerEntry->InnerSetBase(m_pFile, this);
    pInnerEntry->InnerSetMemoryInfo(nEntryRVA, LIBPE_INVALID_ADDRESS, sizeof(LibPERawResourceDirectoryEntry(T)));
    pInnerEntry->InnerSetFileInfo(nEntryFOA, sizeof(LibPERawResourceDirectoryEntry(T)));

    *ppEntry = pInnerEntry.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseResourceDataEntry(PEAddress nRVA, PEAddress nFOA, IPEResourceDataEntry **ppDataEntry)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRVA && LIBPE_INVALID_ADDRESS != nFOA, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppDataEntry, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppDataEntry = NULL;

    LibPEPtr<PEResourceDataEntryT<T>> pInnerDataEntry = new PEResourceDataEntryT<T>();
    if(NULL == pInnerDataEntry) {
        return E_OUTOFMEMORY;
    }

    pInnerDataEntry->InnerSetBase(m_pFile, this);
    pInnerDataEntry->InnerSetMemoryInfo(nRVA, LIBPE_INVALID_ADDRESS, sizeof(LibPERawResourceDataEntry(T)));
    pInnerDataEntry->InnerSetFileInfo(nFOA, sizeof(LibPERawResourceDataEntry(T)));

    *ppDataEntry = pInnerDataEntry.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseResource(IPEResourceDataEntry *pDataEntry, IPEResource **ppResource)
{
    LIBPE_ASSERT_RET(NULL != pDataEntry, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppResource, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppResource = NULL;

    LibPERawResourceDataEntry(T) *pRawDataEntry = (LibPERawResourceDataEntry(T) *)pDataEntry->GetRawMemory();
    LIBPE_ASSERT_RET(NULL != pRawDataEntry, E_FAIL);

    LibPEPtr<PEResourceT<T>> pInnerResource = new PEResourceT<T>();
    if(NULL == pInnerResource) {
        return E_OUTOFMEMORY;
    }

    pInnerResource->InnerSetBase(m_pFile, this);
    pInnerResource->InnerSetMemoryInfo(pRawDataEntry->OffsetToData, LIBPE_INVALID_ADDRESS, pRawDataEntry->Size);
    pInnerResource->InnerSetFileInfo(0, pRawDataEntry->Size);

    *ppResource = pInnerResource.Detach();

    return S_OK;
}

template <class T>
LibPERawResourceString(T) *
PEParserT<T>::ParseResourceString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    PEAddress nRawOffset = GetRawOffset(nRVA, nFOA);
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRawOffset, NULL);

    UINT16 *pRawStringSize = (UINT16 *)m_pLoader->GetBuffer(nRawOffset, sizeof(UINT16));
    if(NULL == pRawStringSize) {
        return NULL;
    }

    if(NULL == m_pLoader->GetBuffer(nRawOffset + sizeof(UINT16), *pRawStringSize)) {
        return NULL;
    }

    return (LibPERawResourceString(T) *)pRawStringSize;
}

template <class T>
LibPERawResourceStringU(T) *
PEParserT<T>::ParseResourceStringU(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);

    PEAddress nRawOffset = GetRawOffset(nRVA, nFOA);
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nRawOffset, NULL);

    UINT16 *pRawStringSize = (UINT16 *)m_pLoader->GetBuffer(nRawOffset, sizeof(UINT16));
    if(NULL == pRawStringSize) {
        return NULL;
    }

    if(NULL == m_pLoader->GetBuffer(nRawOffset + sizeof(UINT16), *pRawStringSize)) {
        return NULL;
    }

    return (LibPERawResourceStringU(T) *)pRawStringSize;
}

template <class T>
HRESULT
PEParserT<T>::ParseExceptionTable(IPEExceptionTable **ppExceptionTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseCertificateTable(IPECertificateTable **ppCertificateTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseRelocationTable(IPERelocationTable **ppRelocationTable)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationTable, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppRelocationTable = NULL;

    PEAddress nRelocationTableRVA = LIBPE_INVALID_ADDRESS, nRelocationTableFOA = LIBPE_INVALID_ADDRESS, nRelocationTableSize = LIBPE_INVALID_SIZE;
    if(FAILED(GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_BASERELOC, nRelocationTableRVA, nRelocationTableFOA, nRelocationTableSize))) {
        return E_FAIL;
    }

    LibPEPtr<PERelocationTableT<T>> pRelocationTable = new PERelocationTableT<T>();
    if(NULL == pRelocationTable) {
        return E_OUTOFMEMORY;
    }

    pRelocationTable->InnerSetBase(m_pFile, this);
    pRelocationTable->InnerSetMemoryInfo(nRelocationTableRVA, LIBPE_INVALID_ADDRESS, nRelocationTableSize);
    pRelocationTable->InnerSetFileInfo(nRelocationTableFOA, nRelocationTableSize);

    LibPERawBaseRelocation(T) *pRawRelocationPage = (LibPERawBaseRelocation(T) *)pRelocationTable->GetRawStruct();
    if(NULL == pRawRelocationPage) {
        return E_OUTOFMEMORY;
    }

    PEAddress nRelocationPageRVA = nRelocationTableRVA;
    PEAddress nRelocationPageFOA = nRelocationTableFOA;
    while(0 != pRawRelocationPage->VirtualAddress) {
        UINT16 *pRawItemList = (UINT16 *)(&pRawRelocationPage[1]);
        UINT32 nItemIndex = 0;
        UINT32 nItemCount = (pRawRelocationPage->SizeOfBlock - sizeof(LibPERawBaseRelocation(T))) / sizeof(UINT16);

        LibPEPtr<PERelocationPageT<T>> pRelocationPage = new PERelocationPageT<T>;
        if(NULL == pRelocationPage) {
            return E_OUTOFMEMORY;
        }

        PEAddress nPageSize = sizeof(LibPERawBaseRelocation(T)) + nItemCount * sizeof(UINT16);

        pRelocationPage->InnerSetBase(m_pFile, this);
        pRelocationPage->InnerSetMemoryInfo(nRelocationPageRVA, LIBPE_INVALID_ADDRESS, nPageSize);
        pRelocationPage->InnerSetFileInfo(nRelocationPageFOA, nPageSize);

        pRelocationTable->InnerAddRelocationPage(pRelocationPage);

        PEAddress nRelocationItemRVA = nRelocationPageRVA + sizeof(LibPERawBaseRelocation(T));
        PEAddress nRelocationItemFOA = nRelocationPageFOA + sizeof(LibPERawBaseRelocation(T));
        while(nItemIndex < nItemCount) {
            LibPEPtr<PERelocationItemT<T>> pRelocationItem = new PERelocationItemT<T>();
            if(NULL == pRelocationItem) {
                return E_OUTOFMEMORY;
            }

            pRelocationItem->InnerSetBase(m_pFile, this);
            pRelocationItem->InnerSetMemoryInfo(nRelocationItemRVA, LIBPE_INVALID_ADDRESS, sizeof(UINT16));
            pRelocationItem->InnerSetFileInfo(nRelocationItemFOA, sizeof(UINT16));
            pRelocationItem->InnerSetRelocateFlag(pRawItemList[nItemIndex] & 0xF000);
            pRelocationItem->InnerSetAddressRVA(pRawRelocationPage->VirtualAddress + (pRawItemList[nItemIndex] & 0x0FFF));
            pRelocationPage->InnerAddRelocationItem(pRelocationItem);

            nRelocationItemRVA += sizeof(UINT16);
            nRelocationItemFOA += sizeof(UINT16);
            ++nItemIndex;
        }

        nRelocationPageRVA = nRelocationItemRVA;
        nRelocationPageFOA = nRelocationItemFOA;
        pRawRelocationPage = (LibPERawBaseRelocation(T) *)(((UINT8 *)pRawRelocationPage) + sizeof(LibPERawBaseRelocation(T)) + sizeof(UINT16) * nItemCount);
    }

    *ppRelocationTable = pRelocationTable.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseGlobalRegister(IPEGlobalRegister **ppGlobalRegister)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseTlsTable(IPETlsTable **ppTlsTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseBoundImportTable(IPEBoundImportTable **ppBoundImportTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportAddressTable(IPEImportAddressTable **ppImportAddressTable)
{
    LIBPE_ASSERT_RET(NULL != ppImportAddressTable, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

    *ppImportAddressTable = NULL;

    PEAddress nImportAddressTableRVA = LIBPE_INVALID_ADDRESS, nImportAddressTableFOA = LIBPE_INVALID_ADDRESS, nImportAddressTableSize = LIBPE_INVALID_SIZE;
    if(FAILED(GetDataDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IAT, nImportAddressTableRVA, nImportAddressTableFOA, nImportAddressTableSize))) {
        return E_FAIL;
    }

    LibPEPtr<PEImportAddressTableT<T>> pImportAddressTable = new PEImportAddressTableT<T>();
    if(NULL == pImportAddressTable) {
        return E_OUTOFMEMORY;
    }

    pImportAddressTable->InnerSetBase(m_pFile, this);
    pImportAddressTable->InnerSetMemoryInfo(nImportAddressTableRVA, LIBPE_INVALID_ADDRESS, nImportAddressTableSize);
    pImportAddressTable->InnerSetFileInfo(nImportAddressTableFOA, nImportAddressTableSize);

    if(FAILED(ParseImportAddressTableContent(pImportAddressTable))) {
        return E_FAIL;
    }

    *ppImportAddressTable = pImportAddressTable.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportAddressTableContent(IPEImportAddressTable *pImportAddressTable)
{
    LIBPE_ASSERT_RET(NULL != pImportAddressTable, E_POINTER);

    LibPERawThunkData(T) *pRawTable = (LibPERawThunkData(T) *)pImportAddressTable->GetRawMemory();
    PEImportAddressTableT<T> *pInnerImportAddressTable = static_cast<PEImportAddressTableT<T> *>(pImportAddressTable);
    PEAddress nTableRVA = pImportAddressTable->GetRVA();
    PEAddress nTableFOA = pImportAddressTable->GetFOA();

    LibPERawThunkData(T) *pRawBlock = pRawTable;
    PEAddress nParsedSize = 0, nTotalSize = pInnerImportAddressTable->GetSizeInFile();

    while(nParsedSize < nTotalSize && 0 != pRawBlock->u1.Function) {
        LibPEPtr<IPEImportAddressBlock> pBlock;
        if(FAILED(ParseImportAddressBlock(pRawBlock, nTableRVA + nParsedSize, nTableFOA + nParsedSize, &pBlock)) || NULL == pBlock) {
            return E_FAIL;
        }
        pInnerImportAddressTable->InnerAddImportAddressBlock(pBlock);

        nParsedSize += pBlock->GetSizeInFile();
        pRawBlock = (LibPERawThunkData(T) *)(((UINT8 *)pRawTable) + nParsedSize);
    }

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, PEAddress nBlockRVA, PEAddress nBlockFOA, IPEImportAddressBlock **ppBlock)
{
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nBlockRVA, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppBlock, E_POINTER);

    *ppBlock = NULL;

    LibPEPtr<PEImportAddressBlockT<T>> pBlock = new PEImportAddressBlockT<T>();
    if(NULL == pBlock) {
        return E_OUTOFMEMORY;
    }

    pBlock->InnerSetBase(m_pFile, this);
    pBlock->InnerSetRawMemory(pRawBlock);
    pBlock->InnerSetMemoryInfo(nBlockRVA, LIBPE_INVALID_ADDRESS, 0);
    pBlock->InnerSetFileInfo(nBlockFOA, 0);

    // Reload FOA to ensure it is avaliable. Because we need it right now.
    if(LIBPE_INVALID_ADDRESS == nBlockFOA) {
        nBlockFOA = GetFOAFromRVA(nBlockRVA);
    }

    // If the RawBlock is NULL, we should ready the memory ourself.
    LibPERawThunkData(T) *pRawItem = NULL;
    BOOL bNeedLoadMemory = false;
    if(NULL == pRawBlock) {
        LIBPE_ASSERT_RET(NULL != m_pLoader, E_FAIL);
        pRawItem = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nBlockFOA, sizeof(LibPERawThunkData(T)));
        LIBPE_ASSERT_RET(NULL != pRawItem, E_OUTOFMEMORY);
        bNeedLoadMemory = true;
    } else {
        pRawItem = pRawBlock;
    }

    // Parse the child import address item. If you don't do this, we will never know the size of this block.
    PEAddress nBlockSize = 0;
    while(pRawItem->u1.Function != 0) {
        LibPEPtr<IPEImportAddressItem> pItem;
        if(FAILED(ParseImportAddressItem(pRawItem, nBlockRVA + nBlockSize, nBlockFOA + nBlockSize, &pItem))) {
            return E_FAIL;
        }
        pBlock->InnerAddImportAddressItem(pItem);

        nBlockSize += sizeof(LibPERawThunkData(T));

        if(bNeedLoadMemory) {
            pRawItem = (LibPERawThunkData(T) *)m_pLoader->GetBuffer(nBlockFOA + nBlockSize, sizeof(LibPERawThunkData(T)));
            LIBPE_ASSERT_RET(NULL != pRawItem, E_OUTOFMEMORY);
        } else {
            ++pRawItem;
        }
    }

    nBlockSize += sizeof(LibPERawThunkData(T));

    pBlock->InnerSetMemoryInfo(nBlockRVA, LIBPE_INVALID_ADDRESS, nBlockSize);
    pBlock->InnerSetFileInfo(nBlockFOA, nBlockSize);

    *ppBlock = pBlock.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, PEAddress nItemRVA, PEAddress nItemFOA, IPEImportAddressItem **ppItem)
{
    // We can ignore the condition - (NULL != pRawItem) here.
    // Because the memory will be ready while you call GetRawStruct() at the first time.
    LIBPE_ASSERT_RET(LIBPE_INVALID_ADDRESS != nItemRVA && LIBPE_INVALID_ADDRESS != nItemFOA, E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != ppItem, E_POINTER);

    *ppItem = NULL;

    LibPEPtr<PEImportAddressItemT<T>> pItem = new PEImportAddressItemT<T>();
    if(NULL == pItem) {
        return E_OUTOFMEMORY;
    }

    pItem->InnerSetBase(m_pFile, this);
    pItem->InnerSetRawMemory(pRawItem);
    pItem->InnerSetMemoryInfo(nItemRVA, LIBPE_INVALID_ADDRESS, sizeof(LibPERawThunkData(T)));
    pItem->InnerSetFileInfo(nItemRVA, sizeof(LibPERawThunkData(T)));

    *ppItem = pItem.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEParserT<T>::ParseDelayImportTable(IPEDelayImportTable **ppDelayImportTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEParserT<T>::ParseCLRHeader(IPECLRHeader **ppCLRHeader)
{
    return E_NOTIMPL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParserT, Create);

LIBPE_NAMESPACE_END