#include "stdafx.h"
#include "PEDumper.h"
#include "PEDosHeaderDumper.h"
#include "PENtHeadersDumper.h"
#include "PESectionHeaderDumper.h"
#include "PESectionDumper.h"
#include "PEExportTableDumper.h"
#include "PEImportTableDumper.h"
#include "PEResourceTableDumper.h"
#include "PEExceptionTableDumper.h"
#include "PECertificateTableDumper.h"
#include "PEDebugInfoTableDumper.h"
#include "PERelocationTableDumper.h"
#include "PEArchitectureDataTableDumper.h"
#include "PEGlobalPointerTableDumper.h"
#include "PETlsTableDumper.h"
#include "PELoadConfigTableDumper.h"
#include "PEBoundImportTableDumper.h"
#include "PEImportAddressTableDumper.h"
#include "PEClrTableDumper.h"

template <class Dumper, class PEElement>
static void DumpPEElementSimple(IPEFile *peFile, HRESULT (LIBPE_CALLTYPE IPEFile::*getElementFunc)(PEElement **), tinyxml2::XMLElement *parentXmlElement)
{
    LibPEPtr<PEElement> peElement;
    if (FAILED((peFile->*getElementFunc)(&peElement))) {
        return;
    }

    Dumper()
        .SetDumpElement(peElement)
        .SetPEFile(peFile)
        .SetParentElement(parentXmlElement)
        .Run();
}

template <class Dumper>
static void DumpPEElementCustom(IPEFile *peFile, tinyxml2::XMLElement *parentXmlElement)
{
    Dumper()
        .SetPEFile(peFile)
        .SetParentElement(parentXmlElement)
        .Run();
}

PEDumper::PEDumper()
{
#define DEFINE_PE_DUMPER_SIMPLE(dumper, getElementFunc, peElement)                                        \
    _dumpers.push_back(                                                                                   \
        [](IPEFile *peFile, tinyxml2::XMLElement *parentXMLElement) {                                     \
            DumpPEElementSimple<dumper, peElement>(peFile, &IPEFile::getElementFunc, parentXMLElement);   \
    })

#define DEFINE_PE_DUMPER_CUSTOM(dumper)                     \
    _dumpers.push_back(DumpPEElementCustom<dumper>)

    DEFINE_PE_DUMPER_SIMPLE(PEDosHeaderDumper, GetDosHeader, IPEDosHeader);
    DEFINE_PE_DUMPER_SIMPLE(PENtHeadersDumper, GetNtHeaders, IPENtHeaders);
    DEFINE_PE_DUMPER_CUSTOM(PESectionHeaderListDumper);
    DEFINE_PE_DUMPER_CUSTOM(PESectionListDumper);
    DEFINE_PE_DUMPER_SIMPLE(PEExportTableDumper, GetExportTable, IPEExportTable);
    DEFINE_PE_DUMPER_SIMPLE(PEImportTableDumper, GetImportTable, IPEImportTable);
    DEFINE_PE_DUMPER_SIMPLE(PEResourceTableDumper, GetResourceTable, IPEResourceTable);
    DEFINE_PE_DUMPER_SIMPLE(PEExceptionTableDumper, GetExceptionTable, IPEExceptionTable);
    DEFINE_PE_DUMPER_SIMPLE(PECertificateTableDumper, GetCertificateTable, IPECertificateTable);
    DEFINE_PE_DUMPER_SIMPLE(PERelocationTableDumper, GetRelocationTable, IPERelocationTable);
    DEFINE_PE_DUMPER_SIMPLE(PEDebugInfoTableDumper, GetDebugInfoTable, IPEDebugInfoTable);
    DEFINE_PE_DUMPER_SIMPLE(PEArchitectureDataTableDumper, GetArchitectureDataTable, IPEArchitectureDataTable);
    DEFINE_PE_DUMPER_SIMPLE(PEGlobalPointerTableDumper, GetGlobalPointerTable, IPEGlobalPointerTable);
    DEFINE_PE_DUMPER_SIMPLE(PETlsTableDumper, GetTlsTable, IPETlsTable);
    DEFINE_PE_DUMPER_SIMPLE(PELoadConfigTableDumper, GetLoadConfigTable, IPELoadConfigTable);
    DEFINE_PE_DUMPER_SIMPLE(PEBoundImportTableDumper, GetBoundImportTable, IPEBoundImportTable);
    DEFINE_PE_DUMPER_SIMPLE(PEImportAddressTableDumper, GetImportAddressTable, IPEImportAddressTable);
    DEFINE_PE_DUMPER_SIMPLE(PEClrTableDumper, GetClrTable, IPEClrTable);

#undef DEFINE_PE_DUMPER_SIMPLE
#undef DEFINE_PE_DUMPER_CUSTOM
}

void
PEDumper::Dump(const file_char_t *peFilePath, const file_char_t *resultFilePath)
{
    if (peFilePath == nullptr || resultFilePath == nullptr) {
        return;
    }

    LibPE::LibPEPtr<LibPE::IPEFile> peFile;
    if (FAILED(LibPE::ParsePEFromDiskFile(peFilePath, &peFile))) {
        return;
    }

    auto rootElement = _xmlDoc.NewElement("PE");
    _xmlDoc.InsertFirstChild(rootElement);

    for (auto dumperIt = _dumpers.begin(); dumperIt != _dumpers.end(); ++dumperIt) {
        (*dumperIt)(peFile, rootElement);
    }

    FILE *xmlFile = nullptr;
    errno_t err = _tfopen_s(&xmlFile, resultFilePath, LIBPE_T("w"));
    if (err != 0 || xmlFile == nullptr) {
        return;
    }

    _xmlDoc.SaveFile(xmlFile);
    fclose(xmlFile);
}
