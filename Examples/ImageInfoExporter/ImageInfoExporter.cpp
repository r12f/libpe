#include "stdafx.h"

using namespace LibPE;

void ExportBasicInfo(IPEFile *pFile)
{
    printf("DosHeader: 0x%p\n", pFile->GetRawDosHeader());
    printf("NtHeaders: 0x%p\n", pFile->GetRawNtHeaders());
    printf("FileHeader: 0x%p\n", pFile->GetRawFileHeader());
    printf("OptionalHeader: 0x%p\n", pFile->GetRawOptionalHeader());
    printf("IsX86File: %s\n", pFile->Is32Bit() ? "YES" : "NO");
}

void ExportSection(IPEFile *pFile)
{
    printf("Sections:\n");

    UINT32 nSectionCount = pFile->GetSectionCount();
    for(UINT32 nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        pFile->GetSection(nSectionIndex, &pSection);
        printf("Section #%lu: Name = %s, RVA = 0x%016I64x, SizeInMemory = %I64u, FOA = 0x%016I64x, SizeInFile = %I64u\n",
            nSectionIndex, pSection->GetName(), pSection->GetRVA(), pSection->GetSizeInMemory(),
            pSection->GetFOA(), pSection->GetSizeInFile());
    }

    LibPEPtr<IPEOverlay> pOverlay;
    if(FAILED(pFile->GetOverlay(&pOverlay)) || NULL == pOverlay) {
        printf("No extra data found.\n");
    } else {
        printf("Extra data: RVA = 0x%08x, SizeInMemory = 0x%08x, FOA = 0x%08x, SizeInFile = 0x%08x\n",
            pOverlay->GetRVA(), pOverlay->GetSizeInMemory(),
            pOverlay->GetFOA(), pOverlay->GetSizeInFile());
    }

    printf("\n");
}

void ExportExportTable(IPEFile *pFile) 
{
    // Export Table
    LibPEPtr<IPEExportTable> pExportTable;
    pFile->GetExportTable(&pExportTable);

    printf("Export Table:\n");
    UINT32 nExportFunctionCount = pExportTable->GetFunctionCount();
    for(UINT32 nExportFunctionIndex = 0; nExportFunctionIndex < nExportFunctionCount; ++nExportFunctionIndex) {
        LibPEPtr<IPEExportFunction> pExportFunction;
        pExportTable->GetFunctionByIndex(nExportFunctionIndex, &pExportFunction);
        printf("Export Function: Name = %s, Ordinal = %d, RVA = 0x%016I64x\n", pExportFunction->GetName(), pExportFunction->GetOrdinal(), pExportFunction->GetRVA());
    }

    printf("\n");
}

void ExportImportTable(IPEFile *pFile) 
{
    // Import Table
    LibPEPtr<IPEImportTable> pImportTable;
    pFile->GetImportTable(&pImportTable);

    printf("Import Table:\n");
    UINT32 nImportModuleCount = pImportTable->GetModuleCount();
    for(UINT32 nImportModuleIndex = 0; nImportModuleIndex < nImportModuleCount; ++nImportModuleIndex) {
        LibPEPtr<IPEImportModule> pImportModule;
        pImportTable->GetModuleByIndex(nImportModuleIndex, &pImportModule);

        LibPEPtr<IPEImportAddressBlock> pRelatedIABlock;
        pImportModule->GetRelatedImportAddressBlock(&pRelatedIABlock);

        printf("Import Module: %s (Bound: %s, IABlock: %016I64x)\n", pImportModule->GetName(), pImportModule->IsBound() ? "true" : "false", pRelatedIABlock->GetRVA());

        for(UINT32 nImportFunctionIndex = 0; nImportFunctionIndex < pImportModule->GetFunctionCount(); ++nImportFunctionIndex) {
            LibPEPtr<IPEImportFunction> pImportFunction;
            pImportModule->GetFunctionByIndex(nImportFunctionIndex, &pImportFunction);
            printf("Import Function: Name = %s, Ordinal = %d\n", pImportFunction->GetName(), pImportFunction->GetOrdinal());
        }

        printf("\n");
    }
}

void ExportResourceDataEntry(IPEResourceDataEntry *pDataEntry)
{
    LibPEPtr<IPEResource> pResource;
    pDataEntry->GetResource(&pResource);
    printf("Resource: RVA = 0x%016I64x, FOA = 0x%016I64x, Size = %I64u", pResource->GetRVA(), pResource->GetFOA(), pResource->GetSizeInMemory());
}

void ExportResourceDirectory(IPEResourceDirectory *pDirectory);
void ExportResourceDirectoryEntry(IPEResourceDirectoryEntry *pDirectoryEntry)
{
    printf("Resource directory entry: ");
    if(pDirectoryEntry->IsNameId()) {
        printf("Id = %d, ", pDirectoryEntry->GetId());
    } else {
        printf("Name = %ws, ", pDirectoryEntry->GetName());
    }

    if(pDirectoryEntry->IsEntryDirectory()) {
        LibPEPtr<IPEResourceDirectory> pChildDirectory;
        pDirectoryEntry->GetDirectory(&pChildDirectory);
        printf("DirectoryRVA = 0x%016I64x\n", pChildDirectory->GetRVA());
        ExportResourceDirectory(pChildDirectory);
    } else {
        LibPEPtr<IPEResourceDataEntry> pChildEntry;
        pDirectoryEntry->GetDataEntry(&pChildEntry);
        printf("DataEntryRVA = 0x%016I64x\n", pChildEntry->GetRVA());
        ExportResourceDataEntry(pChildEntry);
    }
}

void ExportResourceDirectory(IPEResourceDirectory *pDirectory)
{
    printf("Resource directory:\n");
    UINT32 nEntryCount = pDirectory->GetEntryCount();
    for(UINT32 nEntryIndex = 0; nEntryIndex < nEntryCount; ++nEntryIndex) {
        LibPEPtr<IPEResourceDirectoryEntry> pDirectoryEntry;
        pDirectory->GetEntryByIndex(nEntryIndex, &pDirectoryEntry);
        ExportResourceDirectoryEntry(pDirectoryEntry);
    }
    printf("\n");
}

void ExportResourceTable(IPEFile *pFile)
{
    printf("Resource table:\n");

    LibPEPtr<IPEResourceTable> pResourceTable;
    pFile->GetResourceTable(&pResourceTable);

    LibPEPtr<IPEResourceDirectory> pDirectory;
    pResourceTable->GetRootDirectory(&pDirectory);

    ExportResourceDirectory(pDirectory);

    return;
}

void ExportRelocationTable(IPEFile *pFile)
{
    LibPEPtr<IPERelocationTable> pRelocationTable;
    pFile->GetRelocationTable(&pRelocationTable);

    printf("Relocation Table:\n");
    UINT32 nRelocationPageCount = pRelocationTable->GetPageCount();
    for(UINT32 nPageIndex = 0; nPageIndex < nRelocationPageCount; ++nPageIndex) {
        LibPEPtr<IPERelocationPage> pRelocationPage;
        pRelocationTable->GetPageByIndex(nPageIndex, &pRelocationPage);
        printf("Relocation Page: RVA = 0x%08x\n", pRelocationPage->GetPageRVA());

        UINT32 nRelocationItemCount = pRelocationPage->GetItemCount();
        for(UINT32 nItemIndex = 0; nItemIndex < nRelocationItemCount; ++nItemIndex) {
            LibPEPtr<IPERelocationItem> pRelocationItem;
            pRelocationPage->GetItemByIndex(nItemIndex, &pRelocationItem);
            printf("Relocation Item: RVA = 0x%016I64x, Address = 0x%016I64x\n", pRelocationItem->GetAddressRVA(), pRelocationItem->GetRawAddressContent());
        }

        printf("\n");
    }
}

void ExportImportAddressTable(IPEFile *pFile)
{
    LibPEPtr<IPEImportAddressTable> pImportAddressTable;
    pFile->GetImportAddressTable(&pImportAddressTable);

    printf("Import Address Table:\n");
    UINT32 nImportAddressBlockCount = pImportAddressTable->GetBlockCount();
    for(UINT32 nBlockIndex = 0; nBlockIndex < nImportAddressBlockCount; ++nBlockIndex) {
        LibPEPtr<IPEImportAddressBlock> pImportAddressBlock;
        pImportAddressTable->GetBlockByIndex(nBlockIndex, &pImportAddressBlock);
        printf("Import Address Block: RVA = 0x%016I64x\n", pImportAddressBlock->GetRVA());

        UINT32 nImportAddressItemCount = pImportAddressBlock->GetItemCount();
        for(UINT32 nItemIndex = 0; nItemIndex < nImportAddressItemCount; ++nItemIndex) {
            LibPEPtr<IPEImportAddressItem> pImportAddressItem;
            pImportAddressBlock->GetItemByIndex(nItemIndex, &pImportAddressItem);
            printf("Import Address Item: RVA = 0x%016I64x, Address = 0x%016I64x\n", pImportAddressItem->GetRVA(), pImportAddressItem->GetFieldAddressOfData());
        }

        printf("\n");
    }
}

int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
    LibPEPtr<IPEFile> pFile;
    ParsePEFromDiskFile(L"C:\\Windows\\system32\\kernel32.dll", &pFile);

    printf("AddRef: %d\n", pFile->AddRef());
    printf("Release: %d\n", pFile->Release());

    ExportBasicInfo(pFile);
    ExportSection(pFile);
    ExportExportTable(pFile);
    ExportImportTable(pFile);
    ExportResourceTable(pFile);
    //ExportRelocationTable(pFile);
    //ExportImportAddressTable(pFile);

    return 0;
}
