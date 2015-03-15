#include "stdafx.h"
#include "PEImportAddressTableDumper.h"

void PEImportAddressTableDumper::DoDump()
{
    DumpBasicInformation(_importAddressTable);

    UINT32 importAddressBlockCount = _importAddressTable->GetBlockCount();
    GetOutputElement()->SetAttribute("import-address-block-count", importAddressBlockCount);

    for (UINT32 importAddressBlockIndex = 0; importAddressBlockIndex < importAddressBlockCount; ++importAddressBlockIndex) {
        LibPEPtr<IPEImportAddressBlock> importAddressBlock;
        if (FAILED(_importAddressTable->GetBlockByIndex(importAddressBlockIndex, &importAddressBlock))) {
            continue;
        }

        PEImportAddressBlockDumper()
            .SetDumpElement(importAddressBlock)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEImportAddressBlockDumper::DoDump()
{
    DumpBasicInformation(_importAddressBlock);

    UINT32 importAddressItemCount = _importAddressBlock->GetItemCount();
    GetOutputElement()->SetAttribute("import-address-item-count", importAddressItemCount);

    for (UINT32 importAddressItemIndex = 0; importAddressItemIndex < importAddressItemCount; ++importAddressItemIndex) {
        LibPEPtr<IPEImportAddressItem> importAddressItem;
        if (FAILED(_importAddressBlock->GetItemByIndex(importAddressItemIndex, &importAddressItem))) {
            continue;
        }

        PEImportAddressItemDumper()
            .SetDumpElement(importAddressItem)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEImportAddressItemDumper::DoDump()
{
    DumpBasicInformation(_importAddressItem);

    BEGIN_DUMP_PE_ELEMENT(_importAddressItem)
        DUMP_RAW_FIELD_SIMPLE(ForwarderString)
        DUMP_RAW_FIELD_SIMPLE(Function)
        DUMP_RAW_FIELD_SIMPLE(Ordinal)
        DUMP_RAW_FIELD_SIMPLE(AddressOfData)
    END_DUMP_PE_ELEMENT()
}

