// tws_historical_data.h 
#pragma once
#include "tws_wrapper.h"

namespace tws {

	// https://interactivebrokers.github.io/tws-api/historical_data.html
	class HistoricalDataWrapper : public Wrapper {
	public:
		std::string headTimeStamp;
		std::vector<Bar> bars;
		bool done = false;
		HistoricalDataWrapper() = default;
		~HistoricalDataWrapper() = default;

		void reset()
		{
			headTimeStamp.clear();
			bars.clear();
			done = false;
		}

		// Override EWrapper methods as needed
		void headTimestamp(int reqId, const std::string& headTimestamp)
		{
			this->headTimeStamp = headTimestamp;
		}
		void historicalData(TickerId reqId, const Bar& bar) override
		{
			bars.push_back(bar);
		}
		void historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) override
		{
			done = true;
		}

		void req(const Contract& contract, const std::string& endDateTime, const std::string& durationStr,
			const std::string& barSizeSetting, const std::string& whatToShow = "TRADES", int useRTH = 1, int formatDate = 1, bool keepUpToDate = false, const TagValueListSPtr& chartOptions = TagValueListSPtr())
		{
			reset();
			connect();
			EReader reader(&client, &signal);
			reader.start();
			client.reqHistoricalData(Id(), contract, endDateTime, durationStr, barSizeSetting, whatToShow, useRTH, formatDate, keepUpToDate, chartOptions);
			while (!done) {
				signal.waitForSignal();
				reader.processMsgs();
			}
		}
	};

} // namespace tws
