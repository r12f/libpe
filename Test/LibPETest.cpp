#include "stdafx.h"

int wmain(int argc, wchar_t* argv[])
{
    LibPE::ParsePE32FromDiskFile(L"", NULL);
	return 0;
}
