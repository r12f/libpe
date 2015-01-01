#pragma once

#include "LibPE.h"

LIBPE_NAMESPACE_BEGIN

class Config
{
public:
    static void SetPELoaderIOBlockSize(UINT64 nMinBlockSize, UINT64 nMaxBlockSize);
    static UINT64 GetPreferredPELoaderIOBlockSize(UINT64 nFileSize);
};

LIBPE_NAMESPACE_END