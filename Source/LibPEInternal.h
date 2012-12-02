#pragma once

#include "LibPE.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_ASSERT(cond)              do { if(!(cond)) { assert(false); } } while(0)
#define LIBPE_ASSERT_RET(cond, ret)     do { if(!(cond)) { assert(false); return (ret); } } while(0)
#define LIBPE_ASSERT_RET_VOID(cond)     do { if(!(cond)) { assert(false); return; } } while(0)

#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(t)                 static t __dummy_class_## t
#define LIBPE_FORCE_TEMPLATE_REDUCTION_FUNCTION(f)              static void *__dummy_function_## f = (void *)&f
#define LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(t, f)     static void *__dummy_class_function_## t ## _ ## f = (void *)&t::f

LIBPE_NAMESPACE_END