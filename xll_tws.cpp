#include <thread>
#include "tws.h"
#include "tws_tick_type.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

using namespace tws;
using namespace xll;

// Convert long long to double. Faithful if LL < 2^53 = 10^16
constexpr double lltod(long long ll)
{
	return static_cast<double>(ll);
}

// Use std::visit(value, visitor) to handle different types of values in the tick data.
template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };
// Convert TWS value to OPER type.
const auto visitor = overloads
{
	[](bool b) { return OPER(b); },
	[](int i) { return OPER(i); },
	[](long l) { return OPER(static_cast<int>(l)); },
	[](long long ll) { return OPER(lltod(ll)); },
	[](double d) { return OPER(d); },
	[](Decimal d) { return OPER(DecimalFunctions::decimalToDouble(d)); },
	[](const char* s) { return OPER(s); },
	[](const std::string& s) { return OPER(s); }
};

inline OPER oContract(const Contract& c)
{
	OPER o({
		OPER(L"conId"), OPER(lltod(c.conId)),
		OPER(L"symbol"), OPER(c.symbol),
		OPER(L"secType"), OPER(c.secType),
		OPER(L"lastTradeDateOrContractMonth"), OPER(c.lastTradeDateOrContractMonth),
		OPER(L"lastTradeDate"), OPER(c.lastTradeDate),
		OPER(L"strike"), OPER(c.strike),
		OPER(L"right"), OPER(c.right),
		OPER(L"multiplier"), OPER(c.multiplier),
		OPER(L"exchange"), OPER(c.exchange),
		OPER(L"primaryExchange"), OPER(c.primaryExchange),
		OPER(L"currency"), OPER(c.currency),
		OPER(L"localSymbol"), OPER(c.localSymbol),
		OPER(L"tradingClass"), OPER(c.tradingClass),
		OPER(L"includeExpired"), OPER(c.includeExpired),
		OPER(L"secIdType"), OPER(c.secIdType),
		OPER(L"secId"), OPER(c.secId),
		OPER(L"description"), OPER(c.description),
		OPER(L"issuerId"), OPER(c.issuerId),
		OPER(L"comboLegsDescrip"), OPER(c.comboLegsDescrip),
		});
	// Ignore comboLegs and deltaNeutralContract for now.
	o.reshape(size(o) / 2, 2);

	return o;
}

enum TickAttribEnum
{
	canAutoExecute = 1,
	pastLimit = 2,
	reOpen = 4
};
constexpr int TickAttribBits(const TickAttrib& attrib)
{
	return attrib.canAutoExecute * canAutoExecute
		+ attrib.pastLimit * pastLimit
		+ attrib.preOpen * reOpen;
}
#ifdef _DEBUG
static_assert(TickAttribBits(TickAttrib{}) == 0);
static_assert(TickAttribBits(TickAttrib{ false, true, true }) == 6);
#endif // _DEBUG
/*
struct TickPrice {
	void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib& attrib) override
	{
	}
};
*/
//struct SymbolSamples
//static Wrapper wrapper; // host, port, clientId, timeout);


AddIn xai_symbol_sample_wrapper(
	Function(XLL_HANDLEX, L"xll_symbol_sample_wrapper", L"\\" CATEGORY L".SymbolSampleWrapper")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to symbol sample wrapper.")
);
HANDLEX WINAPI xll_symbol_sample_wrapper()
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<Wrapper> h_(new SymbolSamplesWrapper());
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

void WINAPI reqMatchingSymbols(OPER&& asyncHandle, SymbolSamplesWrapper* pssw)
{
	try {
		EReader reader(&pssw->client, &pssw->signal);
		reader.start();
		pssw->signal.waitForSignal();
		reader.processMsgs();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		OPER o;
		for (const auto& cr : pssw->symbolResults) {
			o.hstack(OPER(cr.contract.symbol));
		}
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
	Function(XLL_VOID, L"xll_req_matching_symbols", L"\\" CATEGORY L".reqMatchingSymbols")
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
		const auto pssw = h_.as<SymbolSamplesWrapper>();
		ensure(pssw);
		pssw->client.reqMatchingSymbols(pssw->Id, pattern);
		std::thread(reqMatchingSymbols, *asyncHandle, pssw).detach();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}
#if 0
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
#endif // 0