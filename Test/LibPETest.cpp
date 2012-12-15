#include "stdafx.h"

using namespace LibPE;

void TestExportTable(IPEFile *pFile) 
{
    // Export Table
    LibPEPtr<IPEExportTable> pExportTable;
    pFile->GetExportTable(&pExportTable);

    printf("Export Table:\n");
    for(uint32_t nExportFunctionIndex = 0; nExportFunctionIndex < pExportTable->GetExportFunctionCount(); ++nExportFunctionIndex) {
        LibPEPtr<IPEExportFunction> pExportFunction;
        pExportTable->GetExportFunctionByIndex(nExportFunctionIndex, &pExportFunction);
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
    for(uint32_t nImportModuleIndex = 0; nImportModuleIndex < pImportTable->GetImportModuleCount(); ++nImportModuleIndex) {
        LibPEPtr<IPEImportModule> pImportModule;
        pImportTable->GetImportModuleByIndex(nImportModuleIndex, &pImportModule);
        printf("Import Module: %s\n", pImportModule->GetName());

        for(uint32_t nImportFunctionIndex = 0; nImportFunctionIndex < pImportModule->GetImportFunctionCount(); ++nImportFunctionIndex) {
            LibPEPtr<IPEImportFunction> pImportFunction;
            pImportModule->GetImportFunctionByIndex(nImportFunctionIndex, &pImportFunction);
            printf("Import Function: %s\n", pImportFunction->GetName());
        }

        printf("\n");
    }
}

void TestRelocationTable(IPEFile *pFile)
{
    LibPEPtr<IPERelocationTable> pRelocationTable;
    pFile->GetRelocationTable(&pRelocationTable);

    printf("Relocation Table:\n");
    for(uint32_t nPageIndex = 0; nPageIndex < pRelocationTable->GetRelocationPageCount(); ++nPageIndex) {
        LibPEPtr<IPERelocationPage> pRelocationPage;
        pRelocationTable->GetRelocationPageByIndex(nPageIndex, &pRelocationPage);
        printf("Relocation Page: RVA = 0x%08x\n", pRelocationPage->GetPageRVA());

        for(uint32_t nItemIndex = 0; nItemIndex < pRelocationPage->GetRelocationItemCount(); ++nItemIndex) {
            LibPEPtr<IPERelocationItem> pRelocationItem;
            pRelocationPage->GetRelocationItemByIndex(nItemIndex, &pRelocationItem);
            printf("Relocation Item: RVA = 0x%08x, Address = 0x%08x\n", pRelocationItem->GetAddressRVA(), pRelocationItem->GetRawAddressContent());
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

    IMAGE_DOS_HEADER *pDosHeader = pFile->GetDosHeader();
    printf("DosHeader: 0x%08x\n", pDosHeader);
    printf("NtHeaders: 0x%08x\n", pFile->GetNtHeaders());
    printf("FileHeader: 0x%08x\n", pFile->GetFileHeader());
    printf("OptionalHeader: 0x%08x\n", pFile->GetOptionalHeader());

    TestExportTable(pFile);
    TestImportTable(pFile);
    TestRelocationTable(pFile);

	return 0;
}
