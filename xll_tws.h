// xll_tws.h - Excel specific header for TWS API
#pragma once
#include <variant>
#include "tws_error.h"
#include "tws_contract.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

namespace xll {

	inline void tws_error(const tws::Error& ex)
	{
		switch (tws::errorType(ex.errorCode)) {
		case tws::ErrorType::_ERROR:
			XLL_ERROR(ex.what());
			break;
		case tws::ErrorType::_WARNING:
			XLL_WARNING(ex.what());
			break;
		case tws::ErrorType::_INFORMATION:
			XLL_INFORMATION(ex.what());
			break;
		default:
			XLL_ERROR("Unknown error");
		}
	}
	
	inline OPER DecimalNum(Decimal s)
	{
		return OPER(DecimalFunctions::decimalToDouble(s));
	}
	

	// TWS to Excel type
#define TWS_XLL_TYPE(X) \
		X(bool, Bool) \
		X(int, Int) \
		X(long, Num) \
		X(long long, Num) \
		X(const char*, Str) \
		X(double, Num) \
		X(Decimal, Num) \
		X(const std::string&, Str) \

	// Use std::visit(value, visitor) to handle different types of values in the tick data.
	// Value holds TWS callback data.
	using Value = std::variant<bool, int, long, long long, double, Decimal, const char*, std::string>;

	template<class... Ts>
	struct overloads : Ts... { using Ts::operator()...; };
	
	// Convert TWS value to OPER type.
	const auto visitor = overloads
	{
		[](bool b) { return OPER(b); },
		[](int i) { return OPER(i); },
		[](long l) { return OPER(l); },
		[](long long ll) { return OPER(ll); },
		[](double d) { return OPER(d); },
		[](Decimal d) { return DecimalNum(d); },
		[](const char* s) { return OPER(s); },
		[](const std::string& s) { return OPER(s); }
	};

} // namespace xll


