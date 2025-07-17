#include "xll_tws.h"

using namespace tws;
using namespace xll;

AddIn xai_HistoricalData(
	Function(XLL_HANDLEX, L"xll_HistoricalData", L"\\" CATEGORY L".HistoricalData")
	.Arguments({ })
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Create a historical data wrapper instance.")
);
HANDLEX WINAPI xll_HistoricalData()
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<Wrapper> h_(new HistoricalDataWrapper);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return h;
}

AddIn xai_MktData(
	Function(XLL_HANDLEX, L"xll_MktData", L"\\" CATEGORY L".MktData")
	.Arguments({ })
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Create a market data wrapper instance.")
);
HANDLEX WINAPI xll_MktData()
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<Wrapper> h_(new MktDataWrapper);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return h;
}

// should be async
AddIn xai_reqMktData(
	Function(XLL_LPOPER, L"xll_reqMktData", CATEGORY L".reqMktData")
	.Arguments({
		Arg(XLL_HANDLEX, L"hWrapper", L"Handle to the market data wrapper."),
		Arg(XLL_CSTRING4, L"symbol", L"Symbol of the stock or option."),
	})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Request market data for a given symbol.")
);
LPOPER WINAPI xll_reqMktData(HANDLEX hWrapper, const char* symbol)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		handle<Wrapper> h(hWrapper);
		ensure(h);
		auto wrapper = h.as<MktDataWrapper>();
		ensure(wrapper);
		if (!wrapper->client.isConnected()) {
			XLL_ERROR(L"Market data wrapper is not connected.");
			return &o;
		}
		Stock stock(symbol);;
		wrapper->client.reqMktData(1, stock, "4", false, false, nullptr);

		o = symbol;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = ErrNA;
	}

	return &o;
}
