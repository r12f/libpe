#include "stdafx.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

DataLoaderDiskFile::DataLoaderDiskFile()
    : m_hFile(NULL)
    , m_pFileBuffer(NULL)
    , m_nFileSize(0)
    , m_pBlockStatus(NULL)
    , m_nBlockStatusCount(0)
    , m_nBlockSize(0)
{

}

DataLoaderDiskFile::~DataLoaderDiskFile()
{
    Reset();
}

BOOL
DataLoaderDiskFile::LoadFile(const file_t &strPath)
{
    m_hFile = ::CreateFile(strPath.c_str(), FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE == m_hFile) {
        return false;
    }

    LONG nFileSizeHigh = 0;
    DWORD nFileSizeLow = ::SetFilePointer(m_hFile, 0, &nFileSizeHigh, FILE_END);
    m_nFileSize = ((UINT64)nFileSizeHigh) << 32 | nFileSizeLow;
    m_nBlockSize = GetPreferredPELoaderIOBlockSize(m_nFileSize);

    m_pFileBuffer = new INT8[(INT32)m_nFileSize];
    LIBPE_CHK(0 != m_nBlockSize, false);

    nFileSizeHigh = 0;
    ::SetFilePointer(m_hFile, 0, &nFileSizeHigh, FILE_BEGIN);

    m_nBlockStatusCount = (INT32)(m_nFileSize / m_nBlockSize + ((m_nFileSize & (m_nBlockSize - 1)) > 0 ? 1 : 0));
    m_pBlockStatus = new BOOL[m_nBlockStatusCount];
    memset(m_pBlockStatus, 0, m_nBlockStatusCount * sizeof(BOOL));

    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        Reset();
        return false;
    }

    return true;
}

void *
DataLoaderDiskFile::GetBuffer(UINT64 nOffset, UINT64 nSize)
{
    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        return NULL;
    }

    INT32 nStartBlockId = GetBlockId(nOffset);
    if(0 > nStartBlockId) {
        return NULL;
    }

    INT32 nEndBlockId = GetBlockId(nOffset + nSize - 1);
    if(0 > nEndBlockId) {
        nEndBlockId = GetBlockId(m_nFileSize - 1);
        if (0 > nEndBlockId) {
            return NULL;
        }
    }

    for(int nBlockId = nStartBlockId; nBlockId <= nEndBlockId; ++nBlockId) {
        if(!ReadBlock(nBlockId)) {
            return NULL;
        }
    }

    return &(m_pFileBuffer[nOffset]);
}

const char *
DataLoaderDiskFile::GetAnsiString(UINT64 nOffset, UINT64 &nSize)
{
    if(nOffset >= m_nFileSize) {
        return NULL;
    }

    INT32 nStartBlockId = GetBlockId(nOffset), nReadyBlockId = -1;
    if(0 > nStartBlockId) {
        return NULL;
    }

    UINT64 nCurOffset = nOffset, nBlockEnd = 0;
    for(;;) {
        if(nReadyBlockId != nStartBlockId) {
            if(!ReadBlock(nStartBlockId)) {
                break;
            }
            nReadyBlockId = nStartBlockId;
            nBlockEnd = (nStartBlockId + 1) * m_nBlockSize;
        }

        while(nCurOffset < nBlockEnd) {
            if(0 == m_pFileBuffer[nCurOffset]) {
                nSize = nCurOffset - nOffset + 1;
                return (const char *)&(m_pFileBuffer[nOffset]);
            }
            ++nCurOffset;
        }

        ++nStartBlockId;
    }

    return NULL;
}

const wchar_t *
DataLoaderDiskFile::GetUnicodeString(UINT64 nOffset, UINT64 &nSize)
{
    if(nOffset >= m_nFileSize) {
        return NULL;
    }

    INT32 nStartBlockId = GetBlockId(nOffset), nReadyBlockId = -1;
    if(0 > nStartBlockId) {
        return NULL;
    }

    UINT64 nCurOffset = nOffset, nBlockEnd = 0;
    for(;;) {
        if(nReadyBlockId != nStartBlockId) {
            if(!ReadBlock(nStartBlockId)) {
                break;
            }
            nReadyBlockId = nStartBlockId;
            nBlockEnd = (nStartBlockId + 1) * m_nBlockSize;
        }

        while(nCurOffset < nBlockEnd) {
            if(0 == m_pFileBuffer[nCurOffset]) {
                nSize = nCurOffset - nOffset + 1;
                return (const wchar_t *)&(m_pFileBuffer[nOffset]);
            }
            nCurOffset += sizeof(wchar_t);
        }

        ++nStartBlockId;
    }

    return NULL;
}

void
DataLoaderDiskFile::Reset()
{
    if(INVALID_HANDLE_VALUE != m_hFile) {
        ::CloseHandle(m_hFile);
        m_hFile = NULL;
    }

    if(NULL != m_pFileBuffer) {
        delete [] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }

    if(NULL != m_pBlockStatus) {
        delete [] m_pBlockStatus;
        m_pBlockStatus = NULL;
    }

    m_nFileSize = 0;
    m_nBlockStatusCount = 0;
}

INT32
DataLoaderDiskFile::GetBlockId(UINT64 nOffset)
{
    if(nOffset >= m_nFileSize) {
        return -1;
    }

    return (INT32)(nOffset / m_nBlockSize);
}

BOOL
DataLoaderDiskFile::ReadBlock(INT32 nBlockId)
{
    if(0 > nBlockId || nBlockId >= m_nBlockStatusCount) {
        return false;
    }

    if(m_pBlockStatus[nBlockId] != 0) {
        return true;
    }

    UINT64 nReadBegin = nBlockId * m_nBlockSize;
    LONG nReadBeginHigh = ((nReadBegin >> 32) & 0xFFFFFFFF);
    LONG nReadBeginLow = (nReadBegin & 0xFFFFFFFF);
    DWORD nNeedSize = (DWORD)m_nBlockSize;
    if(nBlockId * m_nBlockSize + nNeedSize > m_nFileSize) {
        nNeedSize = (DWORD)(m_nFileSize - nBlockId * m_nBlockSize);
    }

    ::SetFilePointer(m_hFile, nReadBeginLow, &nReadBeginHigh, FILE_BEGIN);

    DWORD nReadSize = 0;
    LPVOID pBuffer = &(m_pFileBuffer[nBlockId * m_nBlockSize]);
    if(!::ReadFile(m_hFile, pBuffer, nNeedSize, &nReadSize, NULL)) {
        return false;
    }

    m_pBlockStatus[nBlockId] = 1;

    return true;
}

LIBPE_NAMESPACE_END