#include <thread>
#include "tws_tws.h"
#include "tws_tick_type.h"
//#define __STDC_WANT_DEC_FP__ 
//#include "libbid/LIBRARY/src/dfp754.h"

using namespace tws;
using namespace xll;

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };
const auto visitor = overloads
{
	[](double d) { return OPER(d); },
	[](Decimal d) { return OPER(static_cast<double>(d)); }, // TODO: use libbid???
	[](const std::string& s) { return OPER(s.c_str()); }
};

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
	}

	return h;
}

// Process messages.
void WINAPI reqMktData(OPER&& asyncHandle, MktDataWrapper* wrapper)
{
	try {
		EReader reader(&wrapper->client, &wrapper->signal);
		reader.start();
		// break if error???
		//while (wrapper->tickData.size() == 0) {
			wrapper->signal.waitForSignal();
			reader.processMsgs();
		//}
		std::this_thread::sleep_for(std::chrono::seconds(1));;
		for (const auto& [id, kv] : wrapper->tickData) {
			OPER o(2, kv.size());
			int i = 0;
			for (const auto& [tick, value] : kv) {
				// Use std::visit to handle different types of values
				o(0, i) = tickName[tick];
				o(1, i) = std::visit(visitor, value);
			}
			o.reshape(size(o) / 2, 2);
			Excel12(xlAsyncReturn, 0, 2, &asyncHandle, &o);
		}
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

// should be async
AddIn xai_reqMktData(
	Function(XLL_VOID, L"xll_reqMktData", CATEGORY L".reqMktData")
	.Arguments({
		Arg(XLL_HANDLEX, L"hWrapper", L"Handle to the market data wrapper."),
		Arg(XLL_CSTRING4, L"symbol", L"Symbol of the stock or option."),
	})
	.Asynchronous()
	.Category(CATEGORY)
	.FunctionHelp(L"Request market data for a given symbol.")
);
void WINAPI xll_reqMktData(HANDLEX hWrapper, const char* symbol, LPOPER asyncHandle)
{
#pragma XLLEXPORT
	try {
		handle<Wrapper> h(hWrapper);
		ensure(h);
		auto wrapper = h.as<MktDataWrapper>();
		ensure(wrapper);
		ensure(wrapper->client.isConnected());
		Stock stock(symbol);
		wrapper->client.reqMarketDataType(MarketDataType::DELAYED_FROZEN);
		wrapper->client.reqMktData(wrapper->orderId, stock, "0,1,2,3,4,5,6,7", false, false, TagValueListSPtr());
		std::thread(reqMktData, *asyncHandle, wrapper).detach();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}
