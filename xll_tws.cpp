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

// https://interactivebrokers.github.io/tws-api/basic_contracts.html
const OPER oContractHeader({
	OPER(L"conId"), OPER(L"symbol"),
	OPER(L"secType"),
	OPER(L"lastTradeDateOrContractMonth"),
	OPER(L"lastTradeDate"),
	OPER(L"strike"),
	OPER(L"right"),
	OPER(L"multiplier"),
	OPER(L"exchange"),
	OPER(L"primaryExchange"),
	OPER(L"currency"),
	OPER(L"localSymbol"),
	OPER(L"tradingClass"),
	OPER(L"includeExpired"),
	OPER(L"secIdType"),
	OPER(L"secId"),
	OPER(L"description"),
	OPER(L"issuerId"),
	OPER(L"comboLegsDescrip")
});
inline OPER oContract(const Contract& c)
{
	return OPER({
		OPER(lltod(c.conId)),
		OPER(c.symbol),
		OPER(c.secType),
		OPER(c.lastTradeDateOrContractMonth),
		OPER(c.lastTradeDate),
		OPER(c.strike),
		OPER(c.right),
		OPER(c.multiplier),
		OPER(c.exchange),
		OPER(c.primaryExchange),
		OPER(c.currency),
		OPER(c.localSymbol),
		OPER(c.tradingClass),
		OPER(c.includeExpired),
		OPER(c.secIdType),
		OPER(c.secId),
		OPER(c.description),
		OPER(c.issuerId),
		OPER(c.comboLegsDescrip),
	});
}
// Vlookup 2 column range of key, value
static inline OPER lookup(const OPER& key, const OPER& range)
{
	ensure(rows(range) == 2 || columns(range) == 2);

	return Excel(rows(range) == 2 ? xlfHlookup : xlfVlookup, key, range, 2, false); // exact
}
inline Contract eContract(const OPER& o)
{
	Contract c;

	for (const OPER& key : oContractHeader) {
		if (key == (L"conId")) {
			c.conId = Int(lookup(key, o));
		}
		else if (key == (L"symbol")) {
			c.symbol = lookup(key, o).to_string();
		}
		else if (key == (L"secType")) {
			c.secType = lookup(key, o).to_string();
		}
		else if (key == (L"lastTradeDateOrContractMonth")) {
			c.lastTradeDateOrContractMonth = lookup(key, o).to_string();
		}
		else if (key == (L"lastTradeDate")) {
			c.lastTradeDate = lookup(key, o).to_string();
		}
		else if (key == (L"strike")) {
			c.strike = Num(lookup(key, o));
		}
		else if (key == (L"right")) {
			c.right = lookup(key, o).to_string();
		}
		else if (key == (L"multiplier")) {
			c.multiplier = lookup(key, o).to_string();
		}
		else if (key == (L"exchange")) {
			c.exchange = lookup(key, o).to_string();
		}
		else if (key == (L"primaryExchange")) {
			c.primaryExchange = lookup(key, o).to_string();
		}
		else if (key == (L"currency")) {
			c.currency = lookup(key, o).to_string();
		}
		else if (key == (L"localSymbol")) {
			c.localSymbol = lookup(key, o).to_string();
		}
		else if (key == (L"tradingClass")) {
			c.tradingClass = lookup(key, o).to_string();
		}
		else if (key == (L"includeExpired")) {
			c.includeExpired = static_cast<bool>(lookup(key, o));
		}
		else if (key == (L"secIdType")) {
			c.secIdType = lookup(key, o).to_string();
		}
		else if (key == (L"secId")) {
			c.secId = lookup(key, o).to_string();
		}
		else if (key == (L"description")) {
			c.description = lookup(key, o).to_string();
		}
		else if (key == (L"issuerId")) {
			c.issuerId = lookup(key, o).to_string();
		}
		else if (key == (L"comboLegsDescrip")) {
			c.comboLegsDescrip = lookup(key, o).to_string();
		}
	}

	return c;
}

const OPER oContractDetailsHeader({
	// Contract
	OPER(L"marketName"),
	OPER(L"minTick"),
	OPER(L"orderTypes"),
	OPER(L"validExchanges"),
	OPER(L"priceMagnifier"),
	OPER(L"underConId"),
	OPER(L"longName"),
	OPER(L"contractMonth"),
	OPER(L"industry"),
	OPER(L"category"),
	OPER(L"subcategory"),
	OPER(L"timeZoneId"),
	OPER(L"tradingHours"),
	OPER(L"liquidHours"),
	OPER(L"evRule"),
	OPER(L"evMultiplier"),
	OPER(L"aggGroup"),
	OPER(L"underSymbol"),
	OPER(L"underSecType"),
	OPER(L"marketRuleIds"),
	OPER(L"realExpirationDate"),
	OPER(L"lastTradeTime"),
	OPER(L"stockType"),
	OPER(L"minSize"),
	OPER(L"sizeIncrement"),
	OPER(L"suggestedSizeIncrement"),
	// more BOND and FUND fields could be added here
	});
inline OPER oContractDetails(const ContractDetails& cd)
{
	return OPER({
		OPER(cd.marketName),
		OPER(cd.minTick),
		OPER(cd.orderTypes),
		OPER(cd.validExchanges),
		OPER(lltod(cd.priceMagnifier)),
		OPER(cd.underConId),
		OPER(cd.longName),
		OPER(cd.contractMonth),
		OPER(cd.industry),
		OPER(cd.category),
		OPER(cd.subcategory),
		OPER(cd.timeZoneId),
		OPER(cd.tradingHours),
		OPER(cd.liquidHours),
		OPER(cd.evRule),
		OPER(cd.evMultiplier),
		OPER(cd.aggGroup),
		OPER(cd.underSymbol),
		OPER(cd.underSecType),
		OPER(cd.marketRuleIds),
		OPER(cd.realExpirationDate),
		OPER(cd.lastTradeTime),
		OPER(cd.stockType),
		OPER(DecimalFunctions::decimalToDouble(cd.minSize)),
		OPER(DecimalFunctions::decimalToDouble(cd.sizeIncrement)),
		OPER(DecimalFunctions::decimalToDouble(cd.suggestedSizeIncrement)),
		});
}
inline ContractDetails eContractDetails(const OPER& o)
{
	ContractDetails cd;

	for (const OPER& key : oContractDetailsHeader) {
		if (key == (L"marketName")) {
			cd.marketName = lookup(key, o).to_string();
		}
		else if (key == (L"minTick")) {
			cd.minTick = Num(lookup(key, o));
		}
		else if (key == (L"orderTypes")) {
			cd.orderTypes = lookup(key, o).to_string();
		}
		else if (key == (L"validExchanges")) {
			cd.validExchanges = lookup(key, o).to_string();
		}
		else if (key == (L"priceMagnifier")) {
			cd.priceMagnifier = Int(lookup(key, o));
		}
		else if (key == (L"underConId")) {
			cd.underConId = Int(lookup(key, o));
		}
		else if (key == (L"longName")) {
			cd.longName = lookup(key, o).to_string();
		}
		else if (key == (L"contractMonth")) {
			cd.contractMonth = lookup(key, o).to_string();
		}
		else if (key == (L"industry")) {
			cd.industry = lookup(key, o).to_string();
		}
		else if (key == (L"category")) {
			cd.category = lookup(key, o).to_string();
		}
		else if (key == (L"subcategory")) {
			cd.subcategory = lookup(key, o).to_string();
		}
		else if (key == (L"timeZoneId")) {
			cd.timeZoneId = lookup(key, o).to_string();
		}
		else if (key == (L"tradingHours")) {
			cd.tradingHours = lookup(key, o).to_string();
		}
		else if (key == (L"liquidHours")) {
			cd.liquidHours = lookup(key, o).to_string();
		}
		else if (key == (L"evRule")) {
			cd.evRule = lookup(key, o).to_string();
		}
		else if (key == (L"evMultiplier")) {
			cd.evMultiplier = Num(lookup(key, o));
		}
		else if (key == (L"aggGroup")) {
			cd.aggGroup = Int(lookup(key, o));
		}
		else if (key == (L"underSymbol")) {
			cd.underSymbol = lookup(key, o).to_string();
		}
		else if (key == (L"underSecType")) {
			cd.underSecType = lookup(key, o).to_string();
		}
		else if (key == (L"marketRuleIds")) {
			cd.marketRuleIds = lookup(key, o).to_string();
		}
		else if (key == (L"realExpirationDate")) {
			cd.realExpirationDate = lookup(key, o).to_string();
		}
		else if (key == (L"lastTradeTime")) {
			cd.lastTradeTime = lookup(key, o).to_string();
		}
		else if (key == (L"stockType")) {
			cd.stockType = lookup(key, o).to_string();
		}
		else if (key == (L"minSize")) {
			cd.minSize = DecimalFunctions::doubleToDecimal(Num(lookup(key, o)));
		}
		else if (key == (L"sizeIncrement")) {
			cd.sizeIncrement = DecimalFunctions::doubleToDecimal(Num(lookup(key, o)));
		}
		else if (key == (L"suggestedSizeIncrement")) {
			cd.suggestedSizeIncrement = DecimalFunctions::doubleToDecimal(Num(lookup(key, o)));
		}
	}

	return cd;
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
	.FunctionHelp(L"Return key-value pairs for a given contract handle.")
);
LPOPER WINAPI xll_contract(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER o;
	try {
		o = ErrNA;
		handle<Contract> h_(h);
		ensure(h_);
		o = oContractHeader;
		o.vstack(oContract(*h_));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	return &o;
}

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
		pssw->reset();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

AddIn xai_contract_details_wrapper(
	Function(XLL_HANDLEX, L"xll_contract_details_wrapper", L"\\" CATEGORY L"ContractDetails")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to contract details wrapper.")
);
HANDLEX WINAPI xll_contract_details_wrapper()
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;
	try {
		handle<Wrapper> h_(new ContractDetailsWrapper());
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	return h;
}

AddIn xai_req_contract_details(
	Function(XLL_LPOPER, L"xll_req_contract_details", CATEGORY L".ContractDetails")
	.Arguments({
		Arg(XLL_HANDLEX, L"handle", L"contract details handle."),
		Arg(XLL_LPOPER, L"contract", L"contract to request details for.")
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Request contract details for a given contract.")
);
LPOPER WINAPI xll_req_contract_details(HANDLEX h, const LPOPER po)
{
#pragma XLLEXPORT
	static OPER o;
	try {
		handle<Wrapper> h_(h);
		ensure(h_);
		const auto pcdw = h_.as<ContractDetailsWrapper>();
		ensure(pcdw);
		Contract c = eContract(*po);
		/*
		pcdw->req(c);
		o = oContractHeader;
		for (const ContractDetails& cd : pcdw->contractDetailsResults) {
			o.vstack(oContract(cd.contract));
		}
		pcdw->reset();
		*/
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	return &o;
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