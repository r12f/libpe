#pragma once

#ifdef SAMPLEDLL_EXPORTS
#define SAMPLEDLL_API __declspec(dllexport)
#else
#define SAMPLEDLL_API __declspec(dllimport)
#endif

int SAMPLEDLL_API SampleFunction1(int n);
int SAMPLEDLL_API SampleFunction2(int n);
void SAMPLEDLL_API SampleFunction3(int n);
