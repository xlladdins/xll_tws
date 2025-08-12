// xll_contract.cpp - TWS Contract 
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
		Arg(XLL_LPOPER, L"contract", L"JSON contract.")
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

AddIn xai_matching_symbols_wrapper(
	Function(XLL_HANDLEX, L"xll_matching_symbols_wrapper", L"\\" CATEGORY L".MatchingSymbols")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to symbol sample wrapper.")
);
HANDLEX WINAPI xll_matching_symbols_wrapper()
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<Wrapper> h_(new MatchingSymbolsWrapper());
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
AddIn xai_req_matching_symbols(
	Function(XLL_LPOPER, L"xll_req_matching_symbols", CATEGORY L".MatchingSymbols")
	.Arguments({
		Arg(XLL_HANDLEX, L"handle", L"symbol sample handle."),
		Arg(XLL_CSTRING4, L"pattern", L"either start of ticker symbol or (for larger strings) company name.")
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Request matching symbols for a given symbol.")
);

LPOPER WINAPI xll_req_matching_symbols(HANDLEX h, const char* pattern)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		handle<Wrapper> h_(h);
		ensure(h_);
		const auto pssw = h_.as<MatchingSymbolsWrapper>();
		ensure(pssw);
		pssw->req(pattern);
		o = oContractHeader;
		for (const ContractDescription& cd : pssw->symbolResults) {
			o.vstack(oContract(cd.contract));
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
