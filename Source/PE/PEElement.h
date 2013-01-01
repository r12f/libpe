#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEElementT :
    public IPEElement
{
public:
    PEElementT()
        : m_pFile(NULL), m_pRawBuffer(NULL), m_nRVA(0), m_nVA(0), m_nSizeInMemory(0), m_nFOA(0), m_nSizeInFile(0)
    {}

    virtual ~PEElementT() {}

    void InnerSetBase(PEFileT<T> *pFile, PEParserT<T> *pParser)
    {
        m_pFile = pFile;
        m_pParser = pParser;
    }

    void InnerSetRawMemory(void *pRawBuffer)
    {
        m_pRawBuffer = pRawBuffer;
    }
    
    void InnerSetMemoryInfo(PEAddress nRVA, PEAddress nVA, PEAddress nSizeInMemory)
    {
        m_nRVA = nRVA;
        m_nVA = nVA;
        m_nSizeInMemory = nSizeInMemory;
    }

    void InnerSetFileInfo(PEAddress nFOA, PEAddress nSizeInFile)
    {
        m_nFOA = nFOA;
        m_nSizeInFile = nSizeInFile;
    }

    // Override IPEElement
    virtual void * LIBPE_CALLTYPE GetRawMemory();
    virtual PEAddress LIBPE_CALLTYPE GetRawOffset();
    virtual PEAddress LIBPE_CALLTYPE GetRawSize();

    virtual PEAddress LIBPE_CALLTYPE GetRVA();
    virtual PEAddress LIBPE_CALLTYPE GetVA();
    virtual PEAddress LIBPE_CALLTYPE GetSizeInMemory() { return m_nSizeInMemory; }

    virtual PEAddress LIBPE_CALLTYPE GetFOA();
    virtual PEAddress LIBPE_CALLTYPE GetSizeInFile() { return m_nSizeInFile; }

protected:
    LibPEPtr<PEParserT<T>>  m_pParser;
    PEFileT<T>              *m_pFile;
    void                    *m_pRawBuffer;
    PEAddress               m_nRVA;
    PEAddress               m_nVA;
    PEAddress               m_nSizeInMemory;
    PEAddress               m_nFOA;
    PEAddress               m_nSizeInFile;
};

typedef PEElementT<PE32> PEElement32;
typedef PEElementT<PE64> PEElement64;

#define DECLARE_PE_ELEMENT(struct_type)                                                                 \
    LIBPE_SINGLE_THREAD_OBJECT()                                                                        \
                                                                                                        \
    virtual void * LIBPE_CALLTYPE GetRawMemory() { return PEElementT<T>::GetRawMemory(); }              \
    virtual PEAddress LIBPE_CALLTYPE GetRawOffset() { return PEElementT<T>::GetRawOffset(); }           \
    virtual PEAddress LIBPE_CALLTYPE GetRawSize() { return PEElementT<T>::GetRawSize(); }               \
    virtual PEAddress LIBPE_CALLTYPE GetRVA() { return PEElementT<T>::GetRVA(); }                       \
    virtual PEAddress LIBPE_CALLTYPE GetVA() { return PEElementT<T>::GetVA(); }                         \
    virtual PEAddress LIBPE_CALLTYPE GetSizeInMemory() { return PEElementT<T>::GetSizeInMemory(); }     \
    virtual PEAddress LIBPE_CALLTYPE GetFOA() { return PEElementT<T>::GetFOA(); }                       \
    virtual PEAddress LIBPE_CALLTYPE GetSizeInFile(){ return PEElementT<T>::GetSizeInFile(); }          \
                                                                                                        \
    struct_type * GetRawStruct() { return (struct_type *)PEElementT<T>::GetRawMemory(); }


#define LIBPE_FIELD_ACCESSOR_EX(FieldType, FuncName, FieldName)     \
    virtual FieldType LIBPE_CALLTYPE Get ## FuncName() {            \
        LIBPE_ASSERT_RET(NULL == GetRawStruct(), 0);                \
        return GetRawStruct()->FieldName;                           \
    }

#define LIBPE_FIELD_ACCESSOR(FieldType, FieldName)  LIBPE_FIELD_ACCESSOR_EX(FieldType, FieldName, FieldName)

#define LIBPE_ARRAY_FIELD_ACCESSOR_EX(FieldType, FuncName, FieldName, FieldSize)    \
    virtual FieldType * LIBPE_CALLTYPE Get ## FuncName ## Buffer() {                \
        LIBPE_ASSERT_RET(NULL == GetRawStruct(), 0);                                \
        return GetRawStruct()->FieldName;                                           \
    }                                                                               \
                                                                                    \
    virtual UINT32 LIBPE_CALLTYPE Get ## FuncName ## ElementCount() {               \
        return FieldSize;                                                           \
    }                                                                               \
                                                                                    \
    virtual FieldType LIBPE_CALLTYPE Get ## FuncName(UINT32 nIndex) {               \
        LIBPE_ASSERT_RET(NULL == GetRawStruct(), 0);                                \
        LIBPE_ASSERT_RET(nIndex < FieldSize, 0);                                    \
        return GetRawStruct()->FieldName[nIndex];                                   \
    }

#define LIBPE_ARRAY_FIELD_ACCESSOR(FieldType, FieldName, FieldSize)  LIBPE_ARRAY_FIELD_ACCESSOR_EX(FieldType, FieldName, FieldName, FieldSize)

LIBPE_NAMESPACE_END