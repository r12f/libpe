#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

#ifdef LIBPE_WINOS

#include <wtypes.h>
#include <WinError.h>

#else

typedef UINT32              HRESULT;

#define SUCCEEDED(hr)       (((HRESULT)(hr)) >= 0)
#define FAILED(hr)          (((HRESULT)(hr)) < 0)

#define S_OK                ((HRESULT)0L)
#define E_FAIL              ((HRESULT)(0x80004005L))
#define E_POINTER           ((HRESULT)(0x80004003L))
#define E_INVALIDARG        ((HRESULT)(0x80070057L))
#define E_NOTIMPL           ((HRESULT)(0x80004001L))
#define E_OUTOFMEMORY       ((HRESULT)(0x80000002L))

#endif

LIBPE_NAMESPACE_END