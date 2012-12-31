#include "stdafx.h"

using namespace LibPE;

void TestBasicInfo(IPEFile *pFile)
{
    IMAGE_DOS_HEADER *pDosHeader = pFile->GetDosHeader();
    printf("DosHeader: 0x%08x\n", pDosHeader);
    printf("NtHeaders: 0x%08x\n", pFile->GetNtHeaders());
    printf("FileHeader: 0x%08x\n", pFile->GetFileHeader());
    printf("OptionalHeader: 0x%08x\n", pFile->GetOptionalHeader());
}

void TestSection(IPEFile *pFile)
{
    printf("Sections:\n");

    uint32_t nSectionCount = pFile->GetSectionCount();
    for(uint32_t nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        pFile->GetSection(nSectionIndex, &pSection);
        printf("Section #%lu: Name = %s, RVA = 0x%08x, SizeInMemory = 0x%08x, FOA = 0x%08x, SizeInFile = 0x%08x\n",
            nSectionIndex, pSection->GetName(), pSection->GetRVA(), pSection->GetSizeInMemory(),
            pSection->GetFOA(), pSection->GetSizeInFile());
    }

    LibPEPtr<IPEOverlay> pOverlay;
    if(ERR_OK != pFile->GetOverlay(&pOverlay) || NULL == pOverlay) {
        printf("No extra data found.\n");
    } else {
        printf("Extra data: RVA = 0x%08x, SizeInMemory = 0x%08x, FOA = 0x%08x, SizeInFile = 0x%08x\n",
            pOverlay->GetRVA(), pOverlay->GetSizeInMemory(),
            pOverlay->GetFOA(), pOverlay->GetSizeInFile());
    }

    printf("\n");
}

void TestExportTable(IPEFile *pFile) 
{
    // Export Table
    LibPEPtr<IPEExportTable> pExportTable;
    pFile->GetExportTable(&pExportTable);

    printf("Export Table:\n");
    uint32_t nExportFunctionCount = pExportTable->GetFunctionCount();
    for(uint32_t nExportFunctionIndex = 0; nExportFunctionIndex < nExportFunctionCount; ++nExportFunctionIndex) {
        LibPEPtr<IPEExportFunction> pExportFunction;
        pExportTable->GetFunctionByIndex(nExportFunctionIndex, &pExportFunction);
        printf("Export Function: Name = %s, Hint = %d, RVA = 0x%08x\n", pExportFunction->GetName(), pExportFunction->GetHint(), pExportFunction->GetRVA());
    }

    printf("\n");
}

void TestImportTable(IPEFile *pFile) 
{
    // Import Table
    LibPEPtr<IPEImportTable> pImportTable;
    pFile->GetImportTable(&pImportTable);

    printf("Export Table:\n");
    uint32_t nImportModuleCount = pImportTable->GetModuleCount();
    for(uint32_t nImportModuleIndex = 0; nImportModuleIndex < nImportModuleCount; ++nImportModuleIndex) {
        LibPEPtr<IPEImportModule> pImportModule;
        pImportTable->GetModuleByIndex(nImportModuleIndex, &pImportModule);

        LibPEPtr<IPEImportAddressBlock> pRelatedIABlock;
        pImportModule->GetRelatedImportAddressBlock(&pRelatedIABlock);

        printf("Import Module: %s (Bound: %s, IABlock: %08x)\n", pImportModule->GetName(), pImportModule->IsBound() ? "true" : "false", pRelatedIABlock->GetRVA());

        for(uint32_t nImportFunctionIndex = 0; nImportFunctionIndex < pImportModule->GetFunctionCount(); ++nImportFunctionIndex) {
            LibPEPtr<IPEImportFunction> pImportFunction;
            pImportModule->GetFunctionByIndex(nImportFunctionIndex, &pImportFunction);
            printf("Import Function: %s\n", pImportFunction->GetName());
        }

        printf("\n");
    }
}

void TestResourceDataEntry(IPEResourceDataEntry *pDataEntry)
{
    LibPEPtr<IPEResource> pResource;
    pDataEntry->GetResource(&pResource);
    printf("Resource: RVA = 0x%08x, FOA = 0x%08x, Size = %lu", pResource->GetRVA(), pResource->GetFOA(), pResource->GetSizeInMemory());
}

void TestResourceDirectory(IPEResourceDirectory *pDirectory);
void TestResourceDirectoryEntry(IPEResourceDirectoryEntry *pDirectoryEntry)
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
        printf("DirectoryRVA = %lu\n", pChildDirectory->GetRVA());
        TestResourceDirectory(pChildDirectory);
    } else {
        LibPEPtr<IPEResourceDataEntry> pChildEntry;
        pDirectoryEntry->GetDataEntry(&pChildEntry);
        printf("DataEntryRVA = %lu\n", pChildEntry->GetRVA());
        TestResourceDataEntry(pChildEntry);
    }
}

void TestResourceDirectory(IPEResourceDirectory *pDirectory)
{
    printf("Resource directory:\n");
    uint32_t nEntryCount = pDirectory->GetEntryCount();
    for(uint32_t nEntryIndex = 0; nEntryIndex < nEntryCount; ++nEntryIndex) {
        LibPEPtr<IPEResourceDirectoryEntry> pDirectoryEntry;
        pDirectory->GetEntryByIndex(nEntryIndex, &pDirectoryEntry);
        TestResourceDirectoryEntry(pDirectoryEntry);
    }
    printf("\n");
}

void TestResourceTable(IPEFile *pFile)
{
    printf("Resource table:\n");

    LibPEPtr<IPEResourceTable> pResourceTable;
    pFile->GetResourceTable(&pResourceTable);

    LibPEPtr<IPEResourceDirectory> pDirectory;
    pResourceTable->GetRootDirectory(&pDirectory);

    TestResourceDirectory(pDirectory);

    return;
}

void TestRelocationTable(IPEFile *pFile)
{
    LibPEPtr<IPERelocationTable> pRelocationTable;
    pFile->GetRelocationTable(&pRelocationTable);

    printf("Relocation Table:\n");
    uint32_t nRelocationPageCount = pRelocationTable->GetPageCount();
    for(uint32_t nPageIndex = 0; nPageIndex < nRelocationPageCount; ++nPageIndex) {
        LibPEPtr<IPERelocationPage> pRelocationPage;
        pRelocationTable->GetPageByIndex(nPageIndex, &pRelocationPage);
        printf("Relocation Page: RVA = 0x%08x\n", pRelocationPage->GetPageRVA());

        uint32_t nRelocationItemCount = pRelocationPage->GetItemCount();
        for(uint32_t nItemIndex = 0; nItemIndex < nRelocationItemCount; ++nItemIndex) {
            LibPEPtr<IPERelocationItem> pRelocationItem;
            pRelocationPage->GetItemByIndex(nItemIndex, &pRelocationItem);
            printf("Relocation Item: RVA = 0x%08x, Address = 0x%08x\n", pRelocationItem->GetAddressRVA(), pRelocationItem->GetRawAddressContent());
        }

        printf("\n");
    }
}

void TestImportAddressTable(IPEFile *pFile)
{
    LibPEPtr<IPEImportAddressTable> pImportAddressTable;
    pFile->GetImportAddressTable(&pImportAddressTable);

    printf("Import Address Table:\n");
    uint32_t nImportAddressBlockCount = pImportAddressTable->GetBlockCount();
    for(uint32_t nBlockIndex = 0; nBlockIndex < nImportAddressBlockCount; ++nBlockIndex) {
        LibPEPtr<IPEImportAddressBlock> pImportAddressBlock;
        pImportAddressTable->GetBlockByIndex(nBlockIndex, &pImportAddressBlock);
        printf("Import Address Block: RVA = 0x%08x\n", pImportAddressBlock->GetRVA());

        uint32_t nImportAddressItemCount = pImportAddressBlock->GetItemCount();
        for(uint32_t nItemIndex = 0; nItemIndex < nImportAddressItemCount; ++nItemIndex) {
            LibPEPtr<IPEImportAddressItem> pImportAddressItem;
            pImportAddressBlock->GetItemByIndex(nItemIndex, &pImportAddressItem);
            printf("Import Address Item: RVA = 0x%08x, Address = 0x%08x\n", pImportAddressItem->GetRVA(), pImportAddressItem->GetRawAddress());
        }

        printf("\n");
    }
}

int wmain(int argc, wchar_t* argv[])
{
    LibPEPtr<IPEFile> pFile;
    ParsePEFromDiskFile(L"C:\\Windows\\system32\\kernel32.dll", &pFile);

    printf("AddRef: %d\n", pFile->AddRef());
    printf("Release: %d\n", pFile->Release());

    TestBasicInfo(pFile);
    TestSection(pFile);
    TestExportTable(pFile);
    TestImportTable(pFile);
    TestResourceTable(pFile);
    TestRelocationTable(pFile);
    TestImportAddressTable(pFile);

    return 0;
}
