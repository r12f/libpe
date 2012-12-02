#pragma once

#include "LibPE.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_ASSERT(cond)              do { if(!(cond)) { assert(false); } } while(0)
#define LIBPE_ASSERT_RET(cond, ret)     do { if(!(cond)) { assert(false); return (ret); } } while(0)
#define LIBPE_ASSERT_RET_VOID(cond)     do { if(!(cond)) { assert(false); return; } } while(0)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f)              static void *__dummy_function_## f = (void *)&f
#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t, f)     static void *__dummy_class_function_## t ## __ ## f = (void *)&t::f
#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t)                               \
    static void __dummy_function_## t(t *dummy) { dummy->t::~t(); }             \
    static void *__dummy_function_ref_ ## t = &__dummy_function_## t

#define LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION(f)                              \
    LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f ## 32);                         \
    LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION_T(f ## 64)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(t, f)                     \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t ## 32, f);                \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION_T(t ## 64, f)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(t)                                 \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t ## 32);                            \
    LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_T(t ## 64)

LIBPE_NAMESPACE_END