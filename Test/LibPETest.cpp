#include "stdafx.h"

int wmain(int argc, wchar_t* argv[])
{
    LibPE::ParsePE32FromDiskFile(L"c:\\windows\\system32\\kernel32.dll", NULL);
	return 0;
}
