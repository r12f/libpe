#include "stdafx.h"

LIBPE_NAMESPACE_BEGIN

enum {
    DEFAULT_IO_MIN_BLOCK_SIZE  = 16 * 1024,
    DEFAULT_IO_MAX_BLOCK_SIZE  = 2 * 1024 * 1024,
    DEFAULT_IO_COUNT           = 3,
};

uint64_t s_nPELoaderMinBlockSize = 0;
uint64_t s_nPELoaderMaxBlockSize = 0;

void LIBPE_CALLTYPE
SetPELoaderIOBlockSize(uint64_t nMinBlockSize, uint64_t nMaxBlockSize)
{
    s_nPELoaderMinBlockSize = nMinBlockSize;
    s_nPELoaderMaxBlockSize = nMaxBlockSize;
}

uint64_t
GetPreferredPELoaderIOBlockSize(uint64_t nFileSize)
{
    uint64_t nMinBlockSize = (s_nPELoaderMaxBlockSize == 0) ? DEFAULT_IO_MIN_BLOCK_SIZE : s_nPELoaderMinBlockSize;
    uint64_t nMaxBlockSize = (s_nPELoaderMaxBlockSize == 0) ? DEFAULT_IO_MAX_BLOCK_SIZE : s_nPELoaderMaxBlockSize;
    uint64_t nBlockSize = nFileSize / DEFAULT_IO_COUNT;
    if(nBlockSize < nMinBlockSize) { return nBlockSize; }
    if(nBlockSize > nMaxBlockSize) { return nMaxBlockSize; }
    return (((nBlockSize & 0x3FFF) != 0) ? ((nBlockSize | 0x3FFF) + 1) : nBlockSize);
}

LIBPE_NAMESPACE_END