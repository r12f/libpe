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

bool_t
DataLoaderDiskFile::LoadFile(const file_t &strPath)
{
    m_hFile = ::CreateFile(strPath.c_str(), FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE == m_hFile) {
        return false;
    }

    LONG nFileSizeHigh = 0;
    DWORD nFileSizeLow = ::SetFilePointer(m_hFile, 0, &nFileSizeHigh, FILE_END);
    m_nFileSize = ((uint64_t)nFileSizeHigh) << 32 | nFileSizeLow;
    m_nBlockSize = GetPreferredPELoaderIOBlockSize(m_nFileSize);

    m_pFileBuffer = new int8_t[(int32_t)m_nFileSize];
    LIBPE_ASSERT_RET(0 != m_nBlockSize, false);

    nFileSizeHigh = 0;
    ::SetFilePointer(m_hFile, 0, &nFileSizeHigh, FILE_BEGIN);

    m_nBlockStatusCount = (int32_t)(m_nFileSize / m_nBlockSize + ((m_nFileSize & (m_nBlockSize - 1)) > 0 ? 1 : 0));
    m_pBlockStatus = new bool_t[m_nBlockStatusCount];
    memset(m_pBlockStatus, 0, m_nBlockStatusCount * sizeof(bool_t));

    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        Reset();
        return false;
    }

    return true;
}

void *
DataLoaderDiskFile::GetBuffer(uint64_t nOffset, uint64_t nSize)
{
    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        return NULL;
    }

    int32_t nStartBlockId = GetBlockId(nOffset), nEndBlockId = GetBlockId(nOffset + nSize);
    if(0 > nStartBlockId || 0 > nEndBlockId) {
        return NULL;
    }

    for(int nBlockId = nStartBlockId; nBlockId <= nEndBlockId; ++nBlockId) {
        if(!ReadBlock(nBlockId)) {
            return NULL;
        }
    }

    return &(m_pFileBuffer[nOffset]);
}

int8_t *
DataLoaderDiskFile::GetAnsiString(uint64_t nOffset, uint64_t &nSize)
{
    if(nOffset >= m_nFileSize) {
        return NULL;
    }

    int32_t nStartBlockId = GetBlockId(nOffset), nReadyBlockId = -1;
    if(0 > nStartBlockId) {
        return NULL;
    }

    uint64_t nCurOffset = nOffset, nBlockEnd = 0;
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
                return &(m_pFileBuffer[nOffset]);
            }
            ++nCurOffset;
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

int32_t
DataLoaderDiskFile::GetBlockId(uint64_t nOffset)
{
    if(nOffset >= m_nFileSize) {
        return -1;
    }

    return (int32_t)(nOffset / m_nBlockSize);
}

bool_t
DataLoaderDiskFile::ReadBlock(int32_t nBlockId)
{
    if(0 > nBlockId || nBlockId >= m_nBlockStatusCount) {
        return false;
    }

    if(m_pBlockStatus[nBlockId] != 0) {
        return true;
    }

    uint64_t nReadBegin = nBlockId * m_nBlockSize;
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