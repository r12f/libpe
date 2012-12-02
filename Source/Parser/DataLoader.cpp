#include "stdafx.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

DataLoaderDiskFile::DataLoaderDiskFile()
    : m_hFile(NULL)
    , m_pFileBuffer(NULL)
    , m_nFileSize(0)
    , m_pBlockStatus(NULL)
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
    m_nFileSize = ((uint64_t)nFileSizeHigh) << 32 & nFileSizeLow;

    m_pFileBuffer = new int8_t[(int)m_nFileSize];
    m_pBlockStatus = new bool_t[(int)(m_nFileSize / FILE_IO_BLOCK_SIZE + ((m_nFileSize & (FILE_IO_BLOCK_SIZE - 1)) > 0 ? 1 : 0))];

    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        Reset();
        return false;
    }

    return true;
}

int8_t *
DataLoaderDiskFile::GetBuffer(uint64_t nOffset, uint64_t nSize)
{
    if(NULL == m_pFileBuffer || NULL == m_pBlockStatus) {
        return NULL;
    }

    int32_t nStartBlockId = GetBlockId(nOffset), nEndBlockId = GetBlockId(nOffset + nSize);
    if(0 > nStartBlockId || 0 > nEndBlockId) {
        return NULL;
    }

    for(int nBlockId = nStartBlockId; nBlockId < nEndBlockId; ++nBlockId) {
        if(!ReadBlock(nBlockId)) {
            return NULL;
        }
    }

    return &(m_pFileBuffer[nOffset]);
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
}

int32_t
DataLoaderDiskFile::GetBlockId(uint64_t nOffset)
{
    if(nOffset >= m_nFileSize) {
        return -1;
    }

    return (int32_t)(nOffset / FILE_IO_BLOCK_SIZE);
}

bool_t
DataLoaderDiskFile::ReadBlock(int32_t nBlockId)
{
    if(m_pBlockStatus[nBlockId] != 0) {
        return true;
    }

    DWORD nReadSize = 0;
    if(!::ReadFile(m_hFile, &(m_pFileBuffer[nBlockId * FILE_IO_BLOCK_SIZE]), FILE_IO_BLOCK_SIZE, &nReadSize, NULL)) {
        return false;
    }

    return true;
}

LIBPE_NAMESPACE_END