// tws_matching_symbols.h - Matching symbols API for TWS
#pragma once
#include "tws_wrapper.h"

namespace tws {
	struct MatchingSymbolsWrapper : public Wrapper {
		std::vector<ContractDescription> symbolResults;
		//std::mutex symbolMutex;
		//std::condition_variable symbolCv;
		bool done = false;
	public:
		MatchingSymbolsWrapper() = default;
		~MatchingSymbolsWrapper() = default;

		void reset()
		{
			symbolResults.clear();
			done = false;
		}
		void symbolSamples(int reqId, const std::vector<ContractDescription>& contractDescriptions) override
		{
			//std::lock_guard<std::mutex> lock(symbolMutex);
			symbolResults = contractDescriptions;
			done = true;
			//symbolCv.notify_all();
		}
		void req(const char* pattern)
		{
			//std::unique_lock<std::mutex> lock(symbolMutex);
			//symbolCv.wait(lock, [this] { return symbolReady; });
			reset();
			connect();
			EReader reader(&client, &signal);
			reader.start();
			client.reqMatchingSymbols(Id(), pattern);
			while (!done) {
				signal.waitForSignal();
				reader.processMsgs();
			}
			//reader.stop();
			/*
			{
				std::lock_guard<std::mutex> lock(symbolMutex);
				symbolReady = false;
			}
			client.reqMatchingSymbols(Id, pattern);
			std::unique_lock<std::mutex> lock(symbolMutex);
			symbolCv.wait(lock, [this] { return symbolReady; });
			return symbolResults;
			*/
		}
	};

} // namespace tws