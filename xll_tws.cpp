#include <thread>
#include "xll_tws.h"

using namespace tws;
using namespace xll;

std::map<TickerId, std::map<TickType, Wrapper::Value>> Wrapper::tickData;

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
		while (wrapper->tickData.size() == 0) {
			wrapper->signal.waitForSignal();
			reader.processMsgs();
		}
		auto a = wrapper->tickData.begin();
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
		//start_event_loop(wrapper);
		Stock stock(symbol);
		wrapper->client.reqMktData(wrapper->orderId, stock, "68", true, false, nullptr);
		std::thread(reqMktData, *asyncHandle, wrapper).detach();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}
