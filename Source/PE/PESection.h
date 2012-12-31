#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PESectionHeaderT :
    public IPESectionHeader,
    public PEElementT<T>
{
public:
    PESectionHeaderT() {}
    virtual ~PESectionHeaderT() {}

    DECLARE_PE_ELEMENT(LibPERawSectionHeaderT(T))

    virtual HRESULT LIBPE_CALLTYPE GetSection(IPESection **ppSection);

private:
    LibPEPtr<IPESection>    m_pSection;
};

template <class T>
class PESectionT :
    public IPESection,
    public PEElementT<T>
{
public:
    PESectionT() : m_pSectionHeader(NULL) {}
    virtual ~PESectionT() {}

    DECLARE_PE_ELEMENT(void)

    void InnerSetSectionHeader(typename PETrait<T>::RawSectionHeader *pSectionHeader) { m_pSectionHeader = pSectionHeader; }

    virtual const char * LIBPE_CALLTYPE GetName();
    virtual HRESULT LIBPE_CALLTYPE GetRelocations();
    virtual HRESULT LIBPE_CALLTYPE GetLineNumbers();
    virtual UINT32 LIBPE_CALLTYPE GetCharacteristics();

    virtual HRESULT LIBPE_CALLTYPE SetName(const char *pName);

private:
    LibPERawSectionHeaderT(T) *m_pSectionHeader;
};

template <class T>
class PEOverlayT :
    public IPEOverlay,
    public PEElementT<T>
{
public:
    PEOverlayT() {}
    virtual ~PEOverlayT() {}

    DECLARE_PE_ELEMENT(void)
};

typedef PESectionHeaderT<PE32>  PESectionHeader32;
typedef PESectionT<PE32>        PESection32;
typedef PEOverlayT<PE32>        PEOverlay32;

typedef PESectionHeaderT<PE64>  PESectionHeader64;
typedef PESectionT<PE64>        PESection64;
typedef PEOverlayT<PE64>        PEOverlay64;

LIBPE_NAMESPACE_END
