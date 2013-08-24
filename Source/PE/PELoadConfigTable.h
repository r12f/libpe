#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PELoadConfigTableT :
    public IPELoadConfigTable,
    public PEElementT<T>
{
public:
    PELoadConfigTableT() {}
    virtual ~PELoadConfigTableT() {}

    DECLARE_PE_ELEMENT(LibPERawLoadConfigDirectory(T))

    LIBPE_FIELD_ACCESSOR(UINT32, Size)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorVersion)
    LIBPE_FIELD_ACCESSOR(UINT32, GlobalFlagsClear)
    LIBPE_FIELD_ACCESSOR(UINT32, GlobalFlagsSet)
    LIBPE_FIELD_ACCESSOR(UINT32, CriticalSectionDefaultTimeout)
    LIBPE_FIELD_ACCESSOR(UINT64, DeCommitFreeBlockThreshold)
    LIBPE_FIELD_ACCESSOR(UINT64, DeCommitTotalFreeThreshold)
    LIBPE_FIELD_ACCESSOR(UINT64, LockPrefixTable)
    LIBPE_FIELD_ACCESSOR(UINT64, MaximumAllocationSize)
    LIBPE_FIELD_ACCESSOR(UINT64, VirtualMemoryThreshold)
    LIBPE_FIELD_ACCESSOR(UINT64, ProcessAffinityMask)
    LIBPE_FIELD_ACCESSOR(UINT32, ProcessHeapFlags)
    LIBPE_FIELD_ACCESSOR(UINT16, CSDVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, Reserved1)
    LIBPE_FIELD_ACCESSOR(UINT64, EditList)
    LIBPE_FIELD_ACCESSOR(UINT64, SecurityCookie)
    LIBPE_FIELD_ACCESSOR(UINT64, SEHandlerTable)
    LIBPE_FIELD_ACCESSOR(UINT64, SEHandlerCount)
    LIBPE_FIELD_ACCESSOR(UINT64, GuardCFCheckFunctionPointer)
    LIBPE_FIELD_ACCESSOR(UINT64, Reserved2)
    LIBPE_FIELD_ACCESSOR(UINT64, GuardCFFunctionTable)
    LIBPE_FIELD_ACCESSOR(UINT64, GuardCFFunctionCount)
    LIBPE_FIELD_ACCESSOR(UINT32, GuardFlags)
};

LIBPE_NAMESPACE_END