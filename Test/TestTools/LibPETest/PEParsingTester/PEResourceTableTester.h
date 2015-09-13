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

    PEResourceDirectoryTester & SetResourceDirectory(_In_ IPEResourceDirectory *resourceDirectory) {
        _resourceDirectory = resourceDirectory;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDirectory(_In_ IPEResourceDirectory *resourceDirectory);

private:
    LibPEPtr<IPEResourceDirectory> _resourceDirectory;
};

class PEResourceDirectoryEntryTester :
    public PEElementTester
{
public:
    PEResourceDirectoryEntryTester() : PEElementTester("ResourceDirectoryEntry") {}

    PEResourceDirectoryEntryTester & SetResourceDirectoryEntry(_In_ IPEResourceDirectoryEntry *resourceDirectoryEntry) {
        _resourceDirectoryEntry = resourceDirectoryEntry;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDirectoryEntry(_In_ IPEResourceDirectoryEntry *resourceDirectoryEntry);

private:
    LibPEPtr<IPEResourceDirectoryEntry> _resourceDirectoryEntry;
};

class PEResourceDataEntryTester :
    public PEElementTester
{
public:
    PEResourceDataEntryTester() : PEElementTester("ResourceDataEntry") {}

    PEResourceDataEntryTester & SetResourceDataEntry(_In_ IPEResourceDataEntry *resourceDataEntry) {
        _resourceDataEntry = resourceDataEntry;
        return *this;
    }

protected:
    void DoTest() override;
    void TestResourceDataEntry(_In_ IPEResourceDataEntry *resourceDataEntry);

private:
    LibPEPtr<IPEResourceDataEntry> _resourceDataEntry;
};

class PEResourceTester :
    public PEElementTester
{
public:
    PEResourceTester() : PEElementTester("Resource") {}

    PEResourceTester & SetResource(_In_ IPEResource *resource) {
        _resource = resource;
        return *this;
    }

protected:
    void DoTest() override;

private:
    LibPEPtr<IPEResource> _resource;
};
