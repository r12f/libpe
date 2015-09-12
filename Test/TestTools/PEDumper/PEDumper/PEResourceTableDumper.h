#pragma once

#include "PEElementDumper.h"

class PEResourceTableDumper :
    public PEElementDumper
{
public:
    PEResourceTableDumper() : PEElementDumper("ResourceTable") {}

    PEResourceTableDumper & SetDumpElement(IPEResourceTable *resourceTable) {
        _resourceTable = resourceTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEResourceTable> _resourceTable;
};

class PEResourceDirectoryDumper :
    public PEElementDumper
{
public:
    PEResourceDirectoryDumper() : PEElementDumper("ResourceDirectory") {}

    PEResourceDirectoryDumper & SetResourceDirectory(IPEResourceDirectory *resourceDirectory) {
        _resourceDirectory = resourceDirectory;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEResourceDirectory> _resourceDirectory;
};

class PEResourceDirectoryEntryDumper :
    public PEElementDumper
{
public:
    PEResourceDirectoryEntryDumper() : PEElementDumper("ResourceDirectoryEntry") {}

    PEResourceDirectoryEntryDumper & SetResourceDirectoryEntry(IPEResourceDirectoryEntry *resourceDirectoryEntry) {
        _resourceDirectoryEntry = resourceDirectoryEntry;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEResourceDirectoryEntry> _resourceDirectoryEntry;
};

class PEResourceDataEntryDumper :
    public PEElementDumper
{
public:
    PEResourceDataEntryDumper() : PEElementDumper("ResourceDataEntry") {}

    PEResourceDataEntryDumper & SetResourceDataEntry(IPEResourceDataEntry *resourceDataEntry) {
        _resourceDataEntry = resourceDataEntry;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEResourceDataEntry> _resourceDataEntry;
};

class PEResourceDumper :
    public PEElementDumper
{
public:
    PEResourceDumper() : PEElementDumper("Resource") {}
    PEResourceDumper & SetResource(IPEResource *resource) { _resource = resource; return *this; }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEResource> _resource;
};
