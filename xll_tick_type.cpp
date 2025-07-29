// xll_tick_type.cpp - Tick Type enumeration
#include "tws_tick_type.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

using namespace xll;

#ifdef _DEBUG
#define TICK_TYPE_ASSERT(name, value, help, url) static_assert(name == value);
TWS_TICK_TYPE(TICK_TYPE_ASSERT)
#undef TICK_TYPE_ASSERT
#endif // _DEBUG

// Excel enumerations for TWS tick types.
#define TICK_TYPE_ENUM(name, value, help, url) XLL_CONST(SHORT, TWS_TICK_##name, value, help, CATEGORY "_ENUM", url)
TWS_TICK_TYPE(TICK_TYPE_ENUM)
#undef TICK_TYPE_ENUM
