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
        : m_pFile(NULL)
        , m_pRawBuffer(NULL)
        , m_nRVA(LIBPE_INVALID_ADDRESS)
        , m_nVA(LIBPE_INVALID_ADDRESS)
        , m_nSizeInMemory(LIBPE_INVALID_SIZE)
        , m_nFOA(LIBPE_INVALID_ADDRESS)
        , m_nSizeInFile(LIBPE_INVALID_SIZE)
        , m_bIsDelayedDataLoaded(false)
        , m_nLoadDelayedDataResult(E_NOT_VALID_STATE)
    {}

    virtual ~PEElementT() {}

    void InnerSetBase(PEFileT<T> *pFile, PEParserT<T> *pParser) {
        m_pFile = pFile;
        m_pParser = pParser;
    }

    void InnerSetRawMemory(void *pRawBuffer) {
        m_pRawBuffer = pRawBuffer;
    }
    
    void InnerSetMemoryInfo(PEAddress nRVA, PEAddress nVA, PEAddress nSizeInMemory) {
        m_nRVA = nRVA;
        m_nVA = nVA;
        m_nSizeInMemory = nSizeInMemory;
    }

    void InnerSetFileInfo(PEAddress nFOA, PEAddress nSizeInFile) {
        m_nFOA = nFOA;
        m_nSizeInFile = nSizeInFile;
    }

    // Override IPEElement
    void * LIBPE_CALLTYPE GetRawMemory() override;
    PEAddress LIBPE_CALLTYPE GetRawOffset() override;
    PEAddress LIBPE_CALLTYPE GetRawSize() override;

    PEAddress LIBPE_CALLTYPE GetRVA() override;
    PEAddress LIBPE_CALLTYPE GetVA() override;
    PEAddress LIBPE_CALLTYPE GetSizeInMemory() override { return m_nSizeInMemory; }

    PEAddress LIBPE_CALLTYPE GetFOA() override;
    PEAddress LIBPE_CALLTYPE GetSizeInFile() override { return m_nSizeInFile; }

    // Parsing and delay data loading support
    HRESULT EnsureDataReady();
    virtual HRESULT OnParsingFinished() { return S_OK; }
    virtual HRESULT LoadDelayedData() { return S_OK; }

protected:
    LibPEPtr<PEParserT<T>>  m_pParser;
    PEFileT<T>              *m_pFile;
    void                    *m_pRawBuffer;
    PEAddress               m_nRVA;
    PEAddress               m_nVA;
    PEAddress               m_nSizeInMemory;
    PEAddress               m_nFOA;
    PEAddress               m_nSizeInFile;
    bool                    m_bIsDelayedDataLoaded;
    HRESULT                 m_nLoadDelayedDataResult;
};

typedef PEElementT<PE32> PEElement32;
typedef PEElementT<PE64> PEElement64;

#define DECLARE_PE_ELEMENT(struct_type)                                                                 \
    LIBPE_SINGLE_THREAD_OBJECT()                                                                        \
                                                                                                        \
    void * LIBPE_CALLTYPE GetRawMemory() override { return PEElementT<T>::GetRawMemory(); }             \
    PEAddress LIBPE_CALLTYPE GetRawOffset() override { return PEElementT<T>::GetRawOffset(); }          \
    PEAddress LIBPE_CALLTYPE GetRawSize() override { return PEElementT<T>::GetRawSize(); }              \
    PEAddress LIBPE_CALLTYPE GetRVA() override { return PEElementT<T>::GetRVA(); }                      \
    PEAddress LIBPE_CALLTYPE GetVA() override { return PEElementT<T>::GetVA(); }                        \
    PEAddress LIBPE_CALLTYPE GetSizeInMemory() override { return PEElementT<T>::GetSizeInMemory(); }    \
    PEAddress LIBPE_CALLTYPE GetFOA() override { return PEElementT<T>::GetFOA(); }                      \
    PEAddress LIBPE_CALLTYPE GetSizeInFile() override{ return PEElementT<T>::GetSizeInFile(); }         \
                                                                                                        \
    struct_type * GetRawStruct() { return (struct_type *)PEElementT<T>::GetRawMemory(); }


#define LIBPE_FIELD_ACCESSOR_EX(FieldType, FuncName, FieldName)     \
    FieldType LIBPE_CALLTYPE GetField ## FuncName() override {      \
        LIBPE_CHK(NULL != GetRawStruct(), 0);                       \
        return GetRawStruct()->FieldName;                           \
    }

#define LIBPE_FIELD_ACCESSOR(FieldType, FieldName)  LIBPE_FIELD_ACCESSOR_EX(FieldType, FieldName, FieldName)

#define LIBPE_ARRAY_FIELD_ACCESSOR_EX(FieldType, FuncName, FieldName, FieldSize)    \
    FieldType * LIBPE_CALLTYPE GetField ## FuncName ## Buffer() override {          \
        LIBPE_CHK(NULL != GetRawStruct(), NULL);                                    \
        return GetRawStruct()->FieldName;                                           \
    }                                                                               \
                                                                                    \
    UINT32 LIBPE_CALLTYPE GetField ## FuncName ## ElementCount() override {         \
        return FieldSize;                                                           \
    }                                                                               \
                                                                                    \
    FieldType LIBPE_CALLTYPE GetField ## FuncName(UINT32 nIndex) override {         \
        LIBPE_CHK(NULL != GetRawStruct(), 0);                                       \
        LIBPE_CHK(nIndex < FieldSize, 0);                                    		\
        return GetRawStruct()->FieldName[nIndex];                                   \
    }

#define LIBPE_ARRAY_FIELD_ACCESSOR(FieldType, FieldName, FieldSize)  LIBPE_ARRAY_FIELD_ACCESSOR_EX(FieldType, FieldName, FieldName, FieldSize)

LIBPE_NAMESPACE_END