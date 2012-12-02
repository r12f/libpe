#pragma once

#include "LibPE.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_ASSERT(cond)              do { if(!cond) { assert(false); } } while(0)
#define LIBPE_ASSERT_RET(cond, ret)     do { if(!cond) { assert(false); return (ret); } } while(0)
#define LIBPE_ASSERT_RET_VOID(cond)     do { if(!cond) { assert(false); return; } } while(0)

LIBPE_NAMESPACE_END