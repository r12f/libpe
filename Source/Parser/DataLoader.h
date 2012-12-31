#pragma once

#include "Parser/PEParserCommon.h"

LIBPE_NAMESPACE_BEGIN

class DataLoader :
    public ILibPEInterface
{
public:
    LIBPE_SINGLE_THREAD_OBJECT();
    virtual PEParserType GetType() = 0;
    virtual UINT64 GetSize() = 0;
    virtual void * GetBuffer(UINT64 nOffset, UINT64 nSize) = 0;
    virtual const char * GetAnsiString(UINT64 nOffset, UINT64 &nSize) = 0;
    virtual const wchar_t * GetUnicodeString(UINT64 nOffset, UINT64 &nSize) = 0;
};

class DataLoaderDiskFile :
    public DataLoader
{
#ifdef LIBPE_WINOS
    typedef HANDLE FileHandle;
#else
    typedef int FileHandle;
#endif

public:
    DataLoaderDiskFile();
    virtual ~DataLoaderDiskFile();

    BOOL LoadFile(const file_t &strPath);

    // Override PELoader
    virtual PEParserType GetType() { return PE_PARSER_TYPE_DISK_FILE; }
    virtual UINT64 GetSize() { return m_nFileSize; }
    virtual void * GetBuffer(UINT64 nOffset, UINT64 nSize);
    virtual const char * GetAnsiString(UINT64 nOffset, UINT64 &nSize);
    virtual const wchar_t * GetUnicodeString(UINT64 nOffset, UINT64 &nSize);

protected:
    void Reset();
    INT32 GetBlockId(UINT64 nOffset);
    BOOL ReadBlock(INT32 nBlockId);

private:
    FileHandle  m_hFile;
    INT8        *m_pFileBuffer;
    UINT64      m_nFileSize;
    BOOL        *m_pBlockStatus;
    INT32       m_nBlockStatusCount;
    UINT64      m_nBlockSize;
};

LIBPE_NAMESPACE_END