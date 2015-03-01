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

PEDumper::PEDumper()
{
    _dumpers.push_back(new PEDosHeaderDumper());
    _dumpers.push_back(new PENtHeadersDumper());
    _dumpers.push_back(new PESectionHeaderListDumper());
    _dumpers.push_back(new PESectionListDumper());
    _dumpers.push_back(new PEExportTableDumper());
    _dumpers.push_back(new PEImportTableDumper());
    _dumpers.push_back(new PEResourceTableDumper());
    _dumpers.push_back(new PEExceptionTableDumper());
    _dumpers.push_back(new PECertificateTableDumper());
    _dumpers.push_back(new PERelocationTableDumper());
    _dumpers.push_back(new PEDebugInfoTableDumper());
    _dumpers.push_back(new PEArchitectureDataTableDumper());
    _dumpers.push_back(new PEGlobalPointerTableDumper());
    _dumpers.push_back(new PETlsTableDumper());
    _dumpers.push_back(new PELoadConfigTableDumper());
    _dumpers.push_back(new PEBoundImportTableDumper());
    _dumpers.push_back(new PEImportAddressTableDumper());
}

PEDumper::~PEDumper()
{
    for (auto dumperIt = _dumpers.begin(); dumperIt != _dumpers.end(); ++dumperIt) {
        delete *dumperIt;
    }
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
        (*dumperIt)->SetPEFile(peFile).SetParentElement(rootElement).Run();
    }

    FILE *xmlFile = nullptr;
    errno_t err = _tfopen_s(&xmlFile, resultFilePath, LIBPE_T("w"));
    if (err != 0 || xmlFile == nullptr) {
        return;
    }

    _xmlDoc.SaveFile(xmlFile);
    fclose(xmlFile);
}
