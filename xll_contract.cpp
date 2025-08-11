// TWS Contract 
// https://interactivebrokers.github.io/tws-api/classIBApi_1_1Contract.html
// https://ibkrcampus.com/campus/ibkr-api-page/twsapi-doc/#contracts
#include "xll_tws.h"
#include "tws_contract.h"

using namespace xll;
using namespace tws;

// https://interactivebrokers.github.io/tws-api/basic_contracts.html
const OPER oContractHeader({
#define XLL_CONTRACT(name, tws, xll) OPER(L#name),
	TWS_CONTRACT(XLL_CONTRACT)
#undef XLL_CONTRACT	
	});

inline OPER oContract(const Contract& c)
{
#define XLL_CONTRACT(name, tws, xll) xll(c.name),
	return OPER({
		TWS_CONTRACT(XLL_CONTRACT)
		});
#undef XLL_CONTRACT
}

inline Contract eContract(const OPER& o)
{
	Contract c;

	for (const OPER& key : oContractHeader) {
		if (false) {}
#define XLL_CONTRACT(name, tws, xll) else if (key == L#name) { c.name = xll(value(key, o)); }
		TWS_CONTRACT(XLL_CONTRACT)
#undef XLL_CONTRACT
	}

	return c;
}

AddIn xai_contract_(
	Function(XLL_HANDLEX, L"xll_contract_", L"\\" CATEGORY L".Contract")
	.Arguments({
		Arg(XLL_LPOPER, L"contract", L"key-value pairs for a contract.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a contract handle from a key-value pair range.")
);
HANDLEX WINAPI xll_contract_(const LPOPER po)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;
	try {
		Contract c = eContract(*po);
		handle<Contract> h_(new Contract(c));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	return h;
}

AddIn xai_contract(
	Function(XLL_LPOPER, L"xll_contract", CATEGORY L".Contract")
	.Arguments({
		Arg(XLL_HANDLEX, L"handle", L"is a handle to a Contract.")
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return key-value pairs for a given contract handle or contract fields if handle is 0.")
);
LPOPER WINAPI xll_contract(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		o = oContractHeader;
		if (h != 0) {
			handle<Contract> h_(h);
			ensure(h_);
			o.vstack(oContract(*h_));
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

