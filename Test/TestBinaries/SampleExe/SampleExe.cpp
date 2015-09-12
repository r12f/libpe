// SampleExe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../SampleDll/SampleDll.h"
#include "../DelayLoadDll/DelayLoadDll.h"

__declspec(thread) int s_tlsInt = 1;

int _tmain(int argc, _TCHAR* argv[])
{
    s_tlsInt = 0;

    SampleFunction(0);
    SampleFunction1(1);
    SampleFunction2(2);
    SampleFunction3(3);

    return 0;
}

