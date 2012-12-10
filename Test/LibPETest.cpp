#include "stdafx.h"

using namespace LibPE;

void TestExportTable(IPEFile32 *pFile) 
{
    // Export Table
    LibPEPtr<IPEExportTable32> pExportTable;
    pFile->GetExportTable(&pExportTable);

    printf("Export Table:\n");
    for(uint32_t nExportFunctionIndex = 0; nExportFunctionIndex < pExportTable->GetExportFunctionCount(); ++nExportFunctionIndex) {
        LibPEPtr<IPEExportFunction32> pExportFunction;
        pExportTable->GetExportFunctionByIndex(nExportFunctionIndex, &pExportFunction);
        printf("Export Function: Name = %s, Hint = %d, RVA = 0x%08x\n", pExportFunction->GetName(), pExportFunction->GetHint(), pExportFunction->GetRVA());
    }

    printf("\n");
}

void TestImportTable(IPEFile32 *pFile) 
{
    // Import Table
    LibPEPtr<IPEImportTable32> pImportTable;
    pFile->GetImportTable(&pImportTable);

    printf("Export Table:\n");
    for(uint32_t nImportModuleIndex = 0; nImportModuleIndex < pImportTable->GetImportModuleCount(); ++nImportModuleIndex) {
        LibPEPtr<IPEImportModule32> pImportModule;
        pImportTable->GetImportModuleByIndex(nImportModuleIndex, &pImportModule);
        printf("Import Module: %s\n", pImportModule->GetName());

        for(uint32_t nImportFunctionIndex = 0; nImportFunctionIndex < pImportModule->GetImportFunctionCount(); ++nImportFunctionIndex) {
            LibPEPtr<IPEImportFunction32> pImportFunction;
            pImportModule->GetImportFunctionByIndex(nImportFunctionIndex, &pImportFunction);
            printf("Import Function: %s\n", pImportFunction->GetName());
        }

        printf("\n");
    }
}


int wmain(int argc, wchar_t* argv[])
{
    LibPEPtr<IPEFile32> pFile;
    ParsePE32FromDiskFile(L"c:\\windows\\system32\\kernel32.dll", &pFile);

    printf("AddRef: %d\n", pFile->AddRef());
    printf("Release: %d\n", pFile->Release());

    IMAGE_DOS_HEADER *pDosHeader = pFile->GetDosHeader();
    printf("DosHeader: 0x%08x\n", pDosHeader);
    printf("NtHeaders: 0x%08x\n", pFile->GetNtHeaders());
    printf("FileHeader: 0x%08x\n", pFile->GetFileHeader());
    printf("OptionalHeader: 0x%08x\n", pFile->GetOptionalHeader());

    TestExportTable(pFile);
    TestImportTable(pFile);

	return 0;
}
