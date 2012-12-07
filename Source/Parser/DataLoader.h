#pragma once

#include "Parser/PEParserCommon.h"

LIBPE_NAMESPACE_BEGIN

class DataLoader :
    public ILibPEInterface
{
public:
    LIBPE_SINGLE_THREAD_OBJECT();
    virtual PEParserType GetType() = 0;
    virtual int8_t * GetBuffer(uint64_t nOffset, uint64_t nSize) = 0;
};

class DataLoaderDiskFile :
    public DataLoader
{
#ifdef LIBPE_WINOS
    typedef HANDLE FileHandle;
#else
    typedef int FileHandle;
#endif

    enum { FILE_IO_BLOCK_SIZE  = 2 * 1024 * 1024 };

public:
    DataLoaderDiskFile();
    virtual ~DataLoaderDiskFile();

    bool_t LoadFile(const file_t &strPath);

    // Override PELoader
    virtual PEParserType GetType() { return PE_PARSER_TYPE_DISK_FILE; }
    virtual int8_t * GetBuffer(uint64_t nOffset, uint64_t nSize);

protected:
    void Reset();
    int32_t GetBlockId(uint64_t nOffset);
    bool_t ReadBlock(int32_t nBlockId);

private:
    FileHandle  m_hFile;
    int8_t      *m_pFileBuffer;
    uint64_t    m_nFileSize;
    bool_t      *m_pBlockStatus;
    int32_t     m_nBlockStatusCount;
};

LIBPE_NAMESPACE_END