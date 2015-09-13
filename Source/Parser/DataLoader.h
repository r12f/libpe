#pragma once

#include "Parser/PEParserCommon.h"

LIBPE_NAMESPACE_BEGIN

class DataLoader :
    public ILibPEInterface
{
public:
    LIBPE_SINGLE_THREAD_OBJECT();
    virtual ~DataLoader() {}
    virtual PEParserType GetType() = 0;
    virtual UINT64 GetSize() = 0;
    virtual void * GetBuffer(_In_ UINT64 nOffset, _In_ UINT64 nSize) = 0;
    virtual const char * GetAnsiString(_In_ UINT64 nOffset, _Out_ UINT64 &nSize) = 0;
    virtual const wchar_t * GetUnicodeString(_In_ UINT64 nOffset, _Out_ UINT64 &nSize) = 0;
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
    PEParserType GetType() override { return PE_PARSER_TYPE_DISK_FILE; }
    UINT64 GetSize() override { return m_nFileSize; }
    void * GetBuffer(_In_ UINT64 nOffset, _In_ UINT64 nSize) override;
    const char * GetAnsiString(_In_ UINT64 nOffset, _Out_ UINT64 &nSize) override;
    const wchar_t * GetUnicodeString(_In_ UINT64 nOffset, _Out_ UINT64 &nSize) override;

protected:
    void Reset();
    INT32 GetBlockId(_In_ UINT64 nOffset);
    BOOL ReadBlock(_In_ INT32 nBlockId);

private:
    FileHandle  m_hFile;
    INT8        *m_pFileBuffer;
    UINT64      m_nFileSize;
    BOOL        *m_pBlockStatus;
    INT32       m_nBlockStatusCount;
    UINT64      m_nBlockSize;
};

LIBPE_NAMESPACE_END