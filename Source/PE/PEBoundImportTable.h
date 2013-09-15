#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEBoundImportTableT :
    public IPEBoundImportTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEBoundImportModule>> BoundImportModuleList;

public:
    PEBoundImportTableT() : m_bIsBoundImportModulesParsed(false) {}
    virtual ~PEBoundImportTableT() {}

    DECLARE_PE_ELEMENT(LibPERawBoundImportDescriptor(T))

    HRESULT InnerAddBoundImportModule(IPEBoundImportModule *pBoundImportModule);

    virtual UINT32 LIBPE_CALLTYPE GetBoundImportModuleCount();
    virtual HRESULT LIBPE_CALLTYPE GetBoundImportModuleByIndex(UINT32 nIndex, IPEBoundImportModule **ppBoundImportModule);

protected:
    HRESULT EnsureBoundImportModulesParsed();

private:
    bool m_bIsBoundImportModulesParsed;
    BoundImportModuleList m_vBoundImportModules;
};

template <class T>
class PEBoundImportModuleT :
    public IPEBoundImportModule,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEBoundForwarder>> BoundForwarderList;

public:
    PEBoundImportModuleT() {}
    virtual ~PEBoundImportModuleT() {}

    DECLARE_PE_ELEMENT(LibPERawBoundImportDescriptor(T))

    HRESULT InnerAddBoundForwarder(IPEBoundForwarder *pBoundForwarder);

    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_FIELD_ACCESSOR(UINT16, OffsetModuleName);
    LIBPE_FIELD_ACCESSOR(UINT16, NumberOfModuleForwarderRefs);

    virtual UINT32 LIBPE_CALLTYPE GetBoundForwarderCount();
    virtual HRESULT LIBPE_CALLTYPE GetBoundForwarderByIndex(UINT32 nIndex, IPEBoundForwarder **ppBoundForwarder);

private:
    BoundForwarderList m_vBoundForwarders;
};

template <class T>
class PEBoundForwarderT :
    public IPEBoundForwarder,
    public PEElementT<T>
{
public:
    PEBoundForwarderT() {}
    virtual ~PEBoundForwarderT() {}

    DECLARE_PE_ELEMENT(LibPERawBoundForwarderRef(T))

    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_FIELD_ACCESSOR(UINT16, OffsetModuleName);
    LIBPE_FIELD_ACCESSOR(UINT16, Reserved);
};

LIBPE_NAMESPACE_END