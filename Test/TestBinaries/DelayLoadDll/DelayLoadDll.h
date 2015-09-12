#pragma once

#ifdef DELAYLOADDLL_EXPORTS
#define DELAYLOADDLL_API __declspec(dllexport)
#else
#define DELAYLOADDLL_API __declspec(dllimport)
#endif

int DELAYLOADDLL_API SampleFunction(int n);
