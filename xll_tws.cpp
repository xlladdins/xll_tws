#include <thread>
#include "tws.h"
#include "tws_tick_type.h"
//#define __STDC_WANT_DEC_FP__ 
//#include "libbid/LIBRARY/src/dfp754.h"

using namespace tws;
using namespace xll;

// Use std::visit(value, visitor) to handle different types of values in the tick data.
template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };
const auto visitor = overloads
{
	[](double d) { return OPER(d); },
	[](Decimal d) { return OPER(DecimalFunctions::decimalToDouble(d)); },
	[](const std::string& s) { return OPER(s.c_str()); }
};

static Wrapper wrapper; // host, port, clientId, timeout);

AddIn xai_Wrapper(
	Function(XLL_HANDLEX, L"xll_Wrapper", L"\\" CATEGORY L".Wrapper")
	.Arguments({ 
		Arg(XLL_CSTRING4, L"host", L"Host name or IP address of the TWS or IB Gateway."),
		Arg(XLL_INT, L"port", L"Port number of the TWS or IB Gateway."),
		Arg(XLL_INT, L"clientId", L"Client ID for the connection."),
		Arg(XLL_INT, L"timeout", L"Timeout in milliseconds for the connection.")
	})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Create a TWS API wrapper instance.")
);
HANDLEX WINAPI xll_Wrapper(const char* host, int port, int clientId, int timeout)
{
#pragma XLLEXPORT
	return to_handle(&wrapper);
}

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

// Process messages.
void WINAPI reqMktData(OPER&& asyncHandle, MktDataWrapper* wrapper)
{
	try {
		EReader reader(&wrapper->client, &wrapper->signal);
		reader.start();
		wrapper->signal.waitForSignal();
		reader.processMsgs();
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
		wrapper->client.reqMktData(wrapper->orderId, stock, "0,1,2,3,4,5,6,7", false, false, TagValueListSPtr());
		std::thread(reqMktData, *asyncHandle, wrapper).detach();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}
