// xll_tws.h - Excel specific header for TWS API
#pragma once
#include "tws_api/Decimal.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

namespace xll {

	// Use std::visit(value, visitor) to handle different types of values in the tick data.
	template<class... Ts>
	struct overloads : Ts... { using Ts::operator()...; };
	// Convert TWS value to OPER type.
	const auto visitor = overloads
	{
		[](bool b) { return OPER(b); },
		[](int i) { return OPER(i); },
		[](long l) { return OPER(static_cast<int>(l)); },
		[](long long ll) { return OPER(static_cast<double>(ll)); },
		[](double d) { return OPER(d); },
		[](Decimal d) { return OPER(DecimalFunctions::decimalToDouble(d)); },
		[](const char* s) { return OPER(s); },
		[](const std::string& s) { return OPER(s); }
	};

	// Value type to hold TWS callback data.
	using Value = std::variant<bool, int, long, long long, const char*, double, Decimal, std::string>;

} // namespace xll


