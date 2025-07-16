// xll_tick_type.cpp - Tick Type enumeration
#include "xll_tws.h"
#include "tws_tick_type.h"

using namespace xll;

#ifdef _DEBUG
#define TICK_TYPE_ASSERT(name, value, help, url) static_assert(name == value);
TWS_TICK_TYPE(TICK_TYPE_ASSERT)
#undef TICK_TYPE_ASSERT
#endif // _DEBUG

#define TICK_TYPE_ENUM(name, value, help, url) XLL_CONST(SHORT, name, value, help, CATEGORY "_ENUM", url)
TWS_TICK_TYPE(TICK_TYPE_ENUM)
#undef TICK_TYPE_ENUM