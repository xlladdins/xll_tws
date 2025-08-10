// xll_tws.h - Trader Work Station API for Excel
#pragma once
#include <condition_variable>
#include <mutex>
#include <variant>
#pragma warning(disable: 4267)
#include "tws_api/DefaultEWrapper.h"
#include "tws_api/EClientSocket.h"
#include "tws_api/EReaderOSSignal.h"
#include "tws_api/EReader.h"
#include "tws_error.h"

// TwsSocketClientErrors.h
#define TWS_CODE_MSG_PAIR(X) \
	X(NO_VALID_ID, "No valid Id"), \
	X(NO_VALID_ERROR_CODE, "No valid error code"), \
	X(SYSTEM_ERROR, "System error"), \
	X(100, "No error."), \
	X(101, "Unknown error."), \
	X(102, "Invalid argument."), \
	X(103, "Not implemented."), \
	X(104, "Not connected."), \
	X(105, "Already connected."), \
	X(106, "Connection failed."), \
	X(107, "Connection timeout."), \
	X(108, "Connection closed by server."), \
	X(109, "Connection reset by peer."), \
	X(110, "Connection refused."), \
	X(111, "Connection aborted."), \
	X(112, "Connection lost."), \
	X(113, "Connection error."), \
	X(501, "Already connected."), \
	X(502, "Couldn't connect to TWS. Confirm that \"Enable ActiveX and Socket Clients\" "), \
	X(503, "The TWS is out of date and must be upgraded."), \
	X(504, "Not connected"), \
	X(505, "Fatal Unknown message id."), \
	X(506, "Unsupported version"), \
	X(507, "Bad message length"), \
	X(508, "Bad message"), \
	X(509, "Exception caught while reading socket - "), \
	X(520, "Failed to create socket"), \
	X(530, "SSL specific error: "), \
	X(579, "Invalid symbol in string - "), \
	X(585, "FA Profile is not supported anymore, use FA Group instead - "), \

namespace tws {

	inline long Id()
	{
		static long id = 1000; // Starting ID
		return ++id;
	}

	// Wrapper class to manage the connection and provide a default EWrapper implementation
	class Wrapper : public DefaultEWrapper {
	public:
		std::string host;
		int port, clientId;
		EReaderOSSignal signal;
		EClientSocket client;
		// Connect to client socket if necessary.
		Wrapper(const char* _host = "127.0.0.1", int _port = 7497, int _clientId = 0, int timeout = 1000/*ms*/)
			: DefaultEWrapper(), host(_host), port(_port), clientId(_clientId), signal(timeout), client(this, &signal)
		{
			client.setConnectOptions("+PACEAPI");
			client.eConnect(host.c_str(), port, clientId);
		}
		/*
		Wrapper(const Wrapper&) = delete; 
		Wrapper& operator=(const Wrapper&) = delete; 
		Wrapper(Wrapper&&) = default; 
		Wrapper& operator=(Wrapper&&) = default; 
		*/
		virtual ~Wrapper()
		{
			if (client.isConnected()) {
				client.eDisconnect();
			}
		}

		bool connect()
		{
			if (!client.isConnected()) {
				client.eConnect(host.c_str(), port, clientId);
			}
			return client.isConnected();
		}	

		void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
		{
			if (errorType(errorCode) == ErrorType::ERROR) {
				throw tws::Error(id, errorTime, errorCode, errorString, advancedOrderRejectJson);
			}
		}

		void nextValidId(long Id)
		{
			Id = Id;
		}
	};

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

	class ContractDetailsWrapper : public Wrapper {
		ContractDetails details;
		bool done = false;
	public:
		void contractDetails(int reqId, const ContractDetails& details) override
		{
			this->details = details;
		}
		void contractDetailsEnd(int reqId) override 
		{
			reqId = reqId;
			done = true;
		}
		void reset()
		{
			done = false;
		}
		void req(const Contract& contract) {
			reset();
			connect();
			EReader reader(&client, &signal);
			reader.start();
			client.reqContractDetails(Id(), contract);
			while (!done) {
				signal.waitForSignal();
				reader.processMsgs();
			}
		}
	};

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

	class MktDataWrapper : public Wrapper {
	public:
		MktDataWrapper()
			: Wrapper()
		{
		}
		~MktDataWrapper() {}
		/*
		void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib& attrib) override
		{
			tickData[tickerId][field] = price;
		}
		void tickSize(TickerId tickerId, TickType field, Decimal size) override
		{
			tickData[tickerId][field] = size;
		}
		void tickString(TickerId tickerId, TickType tickType, const std::string& value) override
		{
			tickData[tickerId][tickType] = value;
		}
		void tickGeneric(TickerId tickerId, TickType tickType, double value) override
		{
			tickData[tickerId][tickType] = value;
		}
		*/
	};

	// https://interactivebrokers.github.io/tws-api/basic_contracts.html
	struct Futures : public Contract
	{
		Futures(std::string_view symbol)
			: Contract()
		{
			this->symbol = symbol;
			this->secType = "FUT";
			this->currency = "USD"; // Default currency
		}
	};

	struct Index : public Contract
	{
		Index(std::string_view symbol)
			: Contract()
		{
			this->symbol = symbol;
			this->secType = "IND";
			this->currency = "USD"; // Default currency
		}
	};

	struct Option : public Contract
	{
		Option(std::string_view symbol)
			: Contract()
		{
			this->symbol = symbol;
			this->secType = "OPT";
			this->currency = "USD"; // Default currency
		}
	};

	struct Stock : public Contract
	{
		Stock(std::string_view symbol)
			: Contract()
		{
			this->symbol = symbol;
			this->secType = "STK";
			this->currency = "USD"; // Default currency
			this->exchange = "SMART"; // Default exchange
		}
	};


} // namespace tws