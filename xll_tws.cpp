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
		OPER(static_cast<double>(cd.priceMagnifier)),
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
		if (key == L"marketName") {
			cd.marketName = lookupStr(key, o);
		}
		else if (key == L"minTick") {
			cd.minTick = lookupNum(key, o);
		}
		else if (key == L"orderTypes") {
			cd.orderTypes = lookupStr(key, o);
		}
		else if (key == L"validExchanges") {
			cd.validExchanges = lookupStr(key, o);
		}
		else if (key == L"priceMagnifier") {
			cd.priceMagnifier = lookupInt(key, o);
		}
		else if (key == L"underConId") {
			cd.underConId = lookupInt(key, o);
		}
		else if (key == L"longName") {
			cd.longName = lookupStr(key, o);
		}
		else if (key == L"contractMonth") {
			cd.contractMonth = lookupStr(key, o);
		}
		else if (key == L"industry") {
			cd.industry = lookupStr(key, o);
		}
		else if (key == L"category") {
			cd.category = lookupStr(key, o);
		}
		else if (key == L"subcategory") {
			cd.subcategory = lookupStr(key, o);
		}
		else if (key == L"timeZoneId") {
			cd.timeZoneId = lookupStr(key, o);
		}
		else if (key == L"tradingHours") {
			cd.tradingHours = lookupStr(key, o);
		}
		else if (key == L"liquidHours") {
			cd.liquidHours = lookupStr(key, o);
		}
		else if (key == L"evRule") {
			cd.evRule = lookupStr(key, o);
		}
		else if (key == L"evMultiplier") {
			cd.evMultiplier = lookupNum(key, o);
		}
		else if (key == L"aggGroup") {
			cd.aggGroup = lookupInt(key, o);
		}
		else if (key == L"underSymbol") {
			cd.underSymbol = lookupStr(key, o);
		}
		else if (key == L"underSecType") {
			cd.underSecType = lookupStr(key, o);
		}
		else if (key == L"marketRuleIds") {
			cd.marketRuleIds = lookupStr(key, o);
		}
		else if (key == L"realExpirationDate") {
			cd.realExpirationDate = lookupStr(key, o);
		}
		else if (key == L"lastTradeTime") {
			cd.lastTradeTime = lookupStr(key, o);
		}
		else if (key == L"stockType") {
			cd.stockType = lookupStr(key, o);
		}
		else if (key == L"minSize") {
			cd.minSize = lookupDecimal(key, o);
		}
		else if (key == L"sizeIncrement") {
			cd.sizeIncrement = lookupDecimal(key, o);
		}
		else if (key == L"suggestedSizeIncrement") {
			cd.suggestedSizeIncrement = lookupDecimal(key, o);
		}
	}

	return cd;
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
		std::this_thread::sleep_for(std::chrono::seconds(1)); // TWS throttling
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
		o = ErrNA;
		handle<Wrapper> h_(h);
		ensure(h_);
		const auto pcdw = h_.as<ContractDetailsWrapper>();
		ensure(pcdw);

		if (isNum(*po)) {
			handle<Contract> c_(Num(*po));
			ensure(c_);	
			pcdw->req(*c_);
		}
		else {
			Contract c = eContract(*po);
			pcdw->req(c);
		}
		o = oContractDetailsHeader;
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