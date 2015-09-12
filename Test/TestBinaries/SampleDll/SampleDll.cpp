// SampleDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SampleDll.h"
#include "../DelayLoadDll/DelayLoadDll.h"

__declspec(thread) int s_tlsInt = 1;

int SAMPLEDLL_API SampleFunction1(int n) {
    s_tlsInt = n;
    return 0;
}

int SAMPLEDLL_API SampleFunction2(int n) {
    return SampleFunction(n);
}

void SAMPLEDLL_API SampleFunction3(int n) {
}