#include "stdafx.h"
#include "PE/PEGlobalPointerTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PEGlobalPointerTableT<T>::GetRelatedSection(_Outptr_ IPESection **ppSection)
{
    LIBPE_CHK(NULL != ppSection, E_POINTER);
    LIBPE_STRICTCHK(NULL != m_pFile);

    *ppSection = NULL;

    // Global pointer should not be enabled, if the global pointer is NULL, because there is
    // no difference between GP enabled or GP disabled.
    if (NULL == GetFieldGlobalPointerRVA()) {
        return E_NOT_SET;
    }

    // Get the first section with IMAGE_SCN_GPREL flag
    LibPEPtr<IPESectionHeader> pSectionHeader;
    UINT32 nSectionCharacteristics = 0;
    UINT32 nSectionCount = m_pFile->GetSectionCount();
    for (UINT32 nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        pSectionHeader = NULL;
        nSectionCharacteristics = 0;

        LIBPE_CHK_HR(m_pFile->GetSectionHeader(nSectionIndex, &pSectionHeader));
        nSectionCharacteristics = pSectionHeader->GetFieldCharacteristics();
        
        if ((nSectionCharacteristics & IMAGE_SCN_GPREL) != 0) {
            return pSectionHeader->GetSection(ppSection);
        }
    }

    return E_NOT_SET;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEGlobalPointerTableT);

LIBPE_NAMESPACE_END
