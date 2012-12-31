#include "stdafx.h"

LIBPE_NAMESPACE_BEGIN

enum {
    DEFAULT_IO_MIN_BLOCK_SIZE  = 16 * 1024,
    DEFAULT_IO_MAX_BLOCK_SIZE  = 2 * 1024 * 1024,
    DEFAULT_IO_COUNT           = 3,
};

UINT64 s_nPELoaderMinBlockSize = 0;
UINT64 s_nPELoaderMaxBlockSize = 0;

void LIBPE_CALLTYPE
SetPELoaderIOBlockSize(UINT64 nMinBlockSize, UINT64 nMaxBlockSize)
{
    s_nPELoaderMinBlockSize = nMinBlockSize;
    s_nPELoaderMaxBlockSize = nMaxBlockSize;
}

UINT64
GetPreferredPELoaderIOBlockSize(UINT64 nFileSize)
{
    UINT64 nMinBlockSize = (s_nPELoaderMaxBlockSize == 0) ? DEFAULT_IO_MIN_BLOCK_SIZE : s_nPELoaderMinBlockSize;
    UINT64 nMaxBlockSize = (s_nPELoaderMaxBlockSize == 0) ? DEFAULT_IO_MAX_BLOCK_SIZE : s_nPELoaderMaxBlockSize;
    UINT64 nBlockSize = nFileSize / DEFAULT_IO_COUNT;
    if(nBlockSize < nMinBlockSize) { return nBlockSize; }
    if(nBlockSize > nMaxBlockSize) { return nMaxBlockSize; }
    return (((nBlockSize & 0x3FFF) != 0) ? ((nBlockSize | 0x3FFF) + 1) : nBlockSize);
}

LIBPE_NAMESPACE_END