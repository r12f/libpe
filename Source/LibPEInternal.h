#pragma once

#include "LibPE.h"
#include "LibPEConfig.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_ASSERT(cond)              do { if(!(cond)) { assert(false); } } while(0)
#define LIBPE_ASSERT_RET(cond, ret)     do { if(!(cond)) { assert(false); return (ret); } } while(0)
#define LIBPE_ASSERT_RET_VOID(cond)     do { if(!(cond)) { assert(false); return; } } while(0)

#define LIBPE_HR_TRY_BEGIN(hr)          \
    do {                                \
        HRESULT &__try_hr = hr;         \
        try 

#define LIBPE_HR_CATCH()                \
        catch(std::bad_alloc &) {       \
            __try_hr = E_OUTOFMEMORY;   \
        } catch (...) {                 \
            __try_hr = E_FAIL;          \
        }                               \

#define LIBPE_HR_TRY_END()              \
        LIBPE_HR_CATCH()                \
    } while (0)

#define LIBPE_HR_TRY_END_RET()          \
        LIBPE_HR_CATCH()                \
        if (FAILED(__try_hr)) { return __try_hr; }  \
    } while (0)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f, trait)              static void *__dummy_function_## f ## _ ## trait = (void *)&f<trait>
#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t, f, trait)     static void *__dummy_class_function_## t ## _ ## f ## _ ## trait = (void *)&t<trait>::f
#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t, trait)                 template class t<trait>;

#define LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION(f)                              \
    LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f, PE32);                         \
    LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f, PE64)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(t, f)                     \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t, f, PE32);                \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t, f, PE64)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(t)                                 \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t, PE32);                            \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t, PE64)

LIBPE_NAMESPACE_END