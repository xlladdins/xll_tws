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
	EClientSocket client(&w);
	auto i = client.eConnect("", 7497, 0, false);

	return 0;
}
