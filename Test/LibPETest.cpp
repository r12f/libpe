#include "stdafx.h"

using namespace LibPE;

int wmain(int argc, wchar_t* argv[])
{
    LibPEPtr<IPEFile32> pFile;
    ParsePE32FromDiskFile(L"c:\\windows\\system32\\kernel32.dll", &pFile);

    printf("AddRef: %d\n", pFile->AddRef());
    printf("Release: %d\n", pFile->Release());
    printf("Is32BitFile: %d\n", pFile->Is32BitFile());

    IMAGE_DOS_HEADER *pDosHeader = pFile->GetDosHeader();
    printf("DosHeader: 0x%08x\n", pDosHeader);
    printf("NtHeaders: 0x%08x\n", pFile->GetNtHeaders());
    printf("FileHeader: 0x%08x\n", pFile->GetFileHeader());
    printf("OptionalHeader: 0x%08x\n", pFile->GetOptionalHeader());

	return 0;
}
