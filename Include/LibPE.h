#pragma once

#include "LibPE32.h"
#include "LibPE64.h"

LIBPE_NAMESPACE_BEGIN

void LIBPE_CALLTYPE SetPELoaderIOBlockSize(uint64_t nMinBlockSize, uint64_t nMaxBlockSize);

LIBPE_NAMESPACE_END