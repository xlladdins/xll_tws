#include "tws_wrapper.h"
#include "xll_tws.h"

using namespace xll;
using namespace tws;

#if 0

// https://interactivebrokers.github.io/tws-api/historical_bars.html#hd_what_to_show
#define HISTORICAL_DATA_TYPES(X) \
X(TRADES, "Data is adjusted for splits, but not dividends."), \
X(MIDPOINT, ""), \
X(BID, ""), \
X(ASK, ""), \
X(BID_ASK, ""), \
X(ADJUSTED_LAST, "Data is adjusted for splits and dividends. "), \
X(HISTORICAL_VOLATILITY, ""), \
X(OPTION_IMPLIED_VOLATILITY, ""), \
X(FEE_RATE, ""), \
X(YIELD_BID, ""), \
X(YIELD_ASK, ""), \
X(YIELD_BID_ASK, ""), \
X(YIELD_LAST, ""), \
X(AGGTRADES, "Data should only be used with Crypto contracts."), \


using namespace tws;
using namespace xll;


AddIn xai_server_version(
	Function(XLL_DOUBLE, L"xll_server_version", L"\\" CATEGORY L".ServerVersion")
	.Arguments({
		Arg(XLL_HANDLEX, L"handle", L"is a handle to an EWrapper.")
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the server version of the TWS or IB Gateway.")
);
double WINAPI xll_server_version(HANDLEX h)
{
#pragma XLLEXPORT
	double version = INVALID_HANDLEX;
	try {
		handle<Wrapper> h_(h);
		ensure(h_);
		ensure(h_->client.isConnected());
		EClient& ec = h_->client;
		version = ec.serverVersion();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	return version;
}
/*
void WINAPI reqMatchingSymbols(OPER&& asyncHandle, MatchingSymbolsWrapper* pssw)
{
	try {
		pssw->req();
		OPER o;
		for (const auto& cr : pssw->symbolResults) {
			o.hstack(OPER(cr.contract.symbol));
		}
		pssw->reset();
		Excel12(xlAsyncReturn, 0, 2, &asyncHandle, &o);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		Excel12(xlAsyncReturn, 0, 2, &asyncHandle, &ErrNA);
	}
	catch (...) {
		XLL_ERROR("reqMktData: unknown exception");
		Excel12(xlAsyncReturn, 0, 2, &asyncHandle, &ErrNA);
	}
}

AddIn xai_req_matching_symbols(
	Function(XLL_VOID, L"xll_req_matching_symbols", CATEGORY L".reqMatchingSymbols")
	.Arguments({
		Arg(XLL_HANDLEX, L"handle", L"symbol sample handle."),
		Arg(XLL_CSTRING4, L"pattern", L"either start of ticker symbol or (for larger strings) company name.")
	})
	.Asynchronous()
	.Category(CATEGORY)
	.FunctionHelp(L"Request matching symbols for a given symbol.")
);
void WINAPI xll_req_matching_symbols(HANDLEX h, const char* pattern, LPOPER asyncHandle)
{
#pragma XLLEXPORT
	try {
		handle<Wrapper> h_(h);
		ensure(h_);
		const auto pssw = h_.as<MatchingSymbolsWrapper>();
		ensure(pssw);
		pssw->client.reqMatchingSymbols(pssw->Id, pattern);
		std::thread(reqMatchingSymbols, *asyncHandle, pssw).detach();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}
*/
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
	}

	return h;
}


XLL_CONST(INT, MarketDataType_REALTIME, MarketDataType::REALTIME, "Real-time market data type.", CATEGORY, "https://interactivebrokers.github.io/tws-api/market_data_type.html");
XLL_CONST(INT, MarketDataType_FROZEN, MarketDataType::FROZEN, "Real-time market data type.", CATEGORY, "https://interactivebrokers.github.io/tws-api/market_data_type.html");
XLL_CONST(INT, MarketDataType_DELAYED, MarketDataType::DELAYED, "Real-time market data type.", CATEGORY, "https://interactivebrokers.github.io/tws-api/market_data_type.html");
XLL_CONST(INT, MarketDataType_DELAYED_FROZEN, MarketDataType::DELAYED_FROZEN, "Real-time market data type.", CATEGORY, "https://interactivebrokers.github.io/tws-api/market_data_type.html");

AddIn xai_MktData(
	Function(XLL_HANDLEX, L"xll_MktData", L"\\" CATEGORY L".MktData")
	.Arguments({
		Arg(XLL_INT, L"type", L"market data type from the MarketDataType_* enumeration.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Create a market data wrapper instance.")
);
HANDLEX WINAPI xll_MktData(MarketDataType type)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<Wrapper> h_(new MktDataWrapper);
		if (type == 0) {
			// Default market data type.
			type = MarketDataType::DELAYED_FROZEN;
		}
		h_->client.reqMarketDataType(type);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
#endif // 0