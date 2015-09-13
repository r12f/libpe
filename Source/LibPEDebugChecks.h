#pragma once

LIBPE_NAMESPACE_BEGIN

#define LIBPE_CHK(cond, ret)                do { if(!(cond)) { return (ret); } } while(0)
#define LIBPE_CHK_RET_VOID(cond)            do { if(!(cond)) { return; } } while(0)
#define LIBPE_CHK_HR(cond)                  do { HRESULT __hr = (cond); if(FAILED(__hr)) { return (__hr); } } while(0)
#define LIBPE_CHK_HR_RET(cond, ret)         do { HRESULT __hr = (cond); if(FAILED(__hr)) { return (ret); } } while(0)
#define LIBPE_CHK_HR_RET_VOID(cond, ret)    do { HRESULT __hr = (cond); if(FAILED(__hr)) { return; } } while(0)

void DebugCrash(LPCSTR fileName, int lineNumber, LPCSTR functionName, LPCSTR format, ...);
#define LIBPE_STRICTCHK(cond)               \
    do {    \
        if (!(cond)) {     \
            DebugCrash(__FILE__, __LINE__, __FUNCTION__, #cond);  \
        }   \
    } while(0)

LIBPE_NAMESPACE_END
