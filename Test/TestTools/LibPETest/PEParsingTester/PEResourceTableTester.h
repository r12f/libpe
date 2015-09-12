#pragma once

#include "PEElementTester.h"

class PEResourceTableTester :
    public PEElementTester
{
public:
    PEResourceTableTester() : PEElementTester("ResourceTable") {}

protected:
    void DoTest() override;
};

class PEResourceDirectoryTester :
    public PEElementTester
{
public:
    PEResourceDirectoryTester() : PEElementTester("ResourceDirectory") {}

    PEResourceDirectoryTester & SetResourceDirectory(IPEResourceDirectory *resourceDirectory) {
        _resourceDirectory = resourceDirectory;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDirectory(IPEResourceDirectory *resourceDirectory);

private:
    LibPEPtr<IPEResourceDirectory> _resourceDirectory;
};

class PEResourceDirectoryEntryTester :
    public PEElementTester
{
public:
    PEResourceDirectoryEntryTester() : PEElementTester("ResourceDirectoryEntry") {}

    PEResourceDirectoryEntryTester & SetResourceDirectoryEntry(IPEResourceDirectoryEntry *resourceDirectoryEntry) {
        _resourceDirectoryEntry = resourceDirectoryEntry;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDirectoryEntry(IPEResourceDirectoryEntry *resourceDirectoryEntry);

private:
    LibPEPtr<IPEResourceDirectoryEntry> _resourceDirectoryEntry;
};

class PEResourceDataEntryTester :
    public PEElementTester
{
public:
    PEResourceDataEntryTester() : PEElementTester("ResourceDataEntry") {}

    PEResourceDataEntryTester & SetResourceDataEntry(IPEResourceDataEntry *resourceDataEntry) {
        _resourceDataEntry = resourceDataEntry;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDataEntry(IPEResourceDataEntry *resourceDataEntry);

private:
    LibPEPtr<IPEResourceDataEntry> _resourceDataEntry;
};

class PEResourceTester :
    public PEElementTester
{
public:
    PEResourceTester() : PEElementTester("Resource") {}

    PEResourceTester & SetResource(IPEResource *resource) {
        _resource = resource;
        return *this;
    }

protected:
    void DoTest() override;

private:
    LibPEPtr<IPEResource> _resource;
};
