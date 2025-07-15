#include "xll_tws.h"

using namespace tws;
using namespace xll;

AddIn xai_HistoricalData(
	Function(XLL_HANDLEX, L"xll_HistoricalData", CATEGORY L".HistoricalData")
	.Arguments({ })
	.Category(CATEGORY)
	.FunctionHelp(L"Create a HistoricalDataWrapper instance.")
);
HANDLEX WINAPI xll_HistoricalData()
{
#pragma XLLEXPORT
	HistoricalDataWrapper w;
	EReaderOSSignal signal(1000);
	EClientSocket client(&w, &signal);
	auto i = client.eConnect("127.0.0.1", 7497, 0, false);

	return 0;
}
