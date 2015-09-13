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
    ~PEBoundImportTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawBoundImportDescriptor(T))

    HRESULT InnerAddBoundImportModule(_In_ IPEBoundImportModule *pBoundImportModule);

    UINT32 LIBPE_CALLTYPE GetBoundImportModuleCount() override;
    HRESULT LIBPE_CALLTYPE GetBoundImportModuleByIndex(_In_ UINT32 nIndex, _Outptr_ IPEBoundImportModule **ppBoundImportModule) override;

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
    ~PEBoundImportModuleT() override {}

    DECLARE_PE_ELEMENT(LibPERawBoundImportDescriptor(T))

    HRESULT InnerAddBoundForwarder(_In_ IPEBoundForwarder *pBoundForwarder);

    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_FIELD_ACCESSOR(UINT16, OffsetModuleName);
    LIBPE_FIELD_ACCESSOR(UINT16, NumberOfModuleForwarderRefs);

    UINT32 LIBPE_CALLTYPE GetBoundForwarderCount() override;
    HRESULT LIBPE_CALLTYPE GetBoundForwarderByIndex(_In_ UINT32 nIndex, _Outptr_ IPEBoundForwarder **ppBoundForwarder) override;

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