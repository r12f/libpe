#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEExportFunctionT :
    public IPEExportFunctionT<T>,
    public PEElementT<T>
{
public:
    PEExportFunctionT() : m_pName(NULL), m_nHint(0) {}
    virtual ~PEExportFunctionT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()

    void SetRawName(const char *pName) { m_pName = pName; }
    void SetRawHint(uint16_t nHint) { m_nHint = nHint; }

    virtual const char * LIBPE_CALLTYPE GetName();
    virtual uint16_t LIBPE_CALLTYPE GetHint();

private:
    const char *    m_pName;
    uint16_t        m_nHint;
};

typedef PEExportFunctionT<PE32> PEExportFunction32;
typedef PEExportFunctionT<PE64> PEExportFunction64;

LIBPE_NAMESPACE_END
