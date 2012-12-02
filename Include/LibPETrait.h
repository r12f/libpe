#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

struct PE32 {};
struct PE64 {};

template <class T>
struct PETrait {};

template <>
struct PETrait<PE32> {
    typedef uint32_t                Address;
    typedef IMAGE_DOS_HEADER        DosHeader;
    typedef IMAGE_NT_HEADERS32      NtHeaders;
    typedef IMAGE_FILE_HEADER       FileHeader;
    typedef IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

template <>
struct PETrait<PE64> {
    typedef uint64_t                Address;
    typedef IMAGE_DOS_HEADER        DosHeader;
    typedef IMAGE_NT_HEADERS64      NtHeaders;
    typedef IMAGE_FILE_HEADER       FileHeader;
    typedef IMAGE_OPTIONAL_HEADER64 OptionalHeader;
};

template <class T> class PEAddressT : public PETrait<T>::Address {};
template <class T> class PEDosHeaderT : public PETrait<T>::DosHeader {};
template <class T> class PENtHeadersT : public PETrait<T>::NtHeaders {};
template <class T> class PEFileHeaderT : public PETrait<T>::FileHeader {};
template <class T> class PEOptionalHeaderT : public PETrait<T>::OptionalHeader {};

LIBPE_NAMESPACE_END