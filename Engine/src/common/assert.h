#pragma once

#include "src/common/error.h"

#define ASSERT(x, m) { if(!(x)) { ERROR_TRACE(ERR_ASSERTION_FAILED, m);  __debugbreak(); } }