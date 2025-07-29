// xll_tws.h - Trader Work Station API for Excel
#pragma once
#include <condition_variable>
#include <mutex>
#include <variant>
#pragma warning(disable: 4267)
#include "tws_api/EWrapper.h"
#include "tws_api/EClientSocket.h"
#include "tws_api/EReaderOSSignal.h"
#include "tws_api/EReader.h"


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

	// Convert time_t to a localtime string using format.
	inline std::string formatTime(time_t t, const char* format) {
		struct tm* timeinfo = localtime(&t);
		char buffer[20];
		strftime(buffer, sizeof(buffer), format, timeinfo);

		return std::string(buffer);
	}
	inline const char* const YMDHMS = "%Y%m%d %H:%M:%S";
	inline const char* const YMD = "%Y%m%d";
	inline std::string DateTime(time_t t) { return formatTime(t, YMDHMS); }
	inline std::string Date(time_t t) { return formatTime(t, YMD); }

	enum class ErrorType {
		Error,
		Warning,
		Information,
		Unknown
	};

	struct Error : public std::exception {
		int id;
		time_t errorTime;
		int errorCode;
		std::string errorString;
		std::string advancedOrderRejectJson;
		Error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson)
			: id(id), errorTime(errorTime), errorCode(errorCode), errorString(errorString), advancedOrderRejectJson(advancedOrderRejectJson) {
		}
		ErrorType errorType() const
		{
			// Error codes (partial list, expand as needed)
			if (100 <= errorCode && errorCode <= 999)
				return ErrorType::Error;
			// Warnings
			if (errorCode == 2104 || errorCode == 2106 || errorCode == 2107 || errorCode == 2108)
				return ErrorType::Warning;
			// Informational
			if (errorCode == 2103 || errorCode == 2105 || errorCode == 2158)
				return ErrorType::Information;
			// Add more codes as needed

			return ErrorType::Unknown;
		}
		const char* what() const override
		{
			return errorString.c_str(); // TODO: include id, errorTime, advancedOrderRejectJson
		}
	};


	// Value type to hold TWS callback data.
	using Value = std::variant<bool, int, long, long long, const char*, double, Decimal, std::string>;

	// Wrapper class to manage the connection and provide a default EWrapper implementation
	class Wrapper : public EWrapper {
	public:
		long Id;
		EReaderOSSignal signal;
		EClientSocket client;
		// Connect to client socket if necessary.
		Wrapper(const char* host = "127.0.0.1", int port = 7497, int clientId = 0, int timeout = 1000/*ms*/)
			: EWrapper(), Id(0), signal(timeout), client(this, &signal)
		{
			//if (!client.isConnected()) {
				client.setConnectOptions("+PACEAPI");
				client.eConnect(host, port, clientId);
			//}
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

		// reset???

		void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
		{
			throw Error(id, errorTime, errorCode, errorString, advancedOrderRejectJson);
		}

		void nextValidId(long Id)
		{
			this->Id = Id;
		}
	};

	struct SymbolSamplesWrapper : public Wrapper {
		std::vector<ContractDescription> symbolResults;
		std::mutex symbolMutex;
		std::condition_variable symbolCv;
		bool symbolReady = false;
 		public:
		SymbolSamplesWrapper() = default;
		~SymbolSamplesWrapper() = default;
		void symbolSamples(int reqId, const std::vector<ContractDescription>& contractDescriptions) override
		{
			std::lock_guard<std::mutex> lock(symbolMutex);
			symbolResults = contractDescriptions;
			symbolReady = true;
			symbolCv.notify_all();
		}
		std::vector<ContractDescription> reqMatchingSymbols(const std::string& pattern) 
		{
			client.reqMatchingSymbols(Id, pattern);
			EReader reader(&client, &signal);
			reader.start();
			for (int i = 0; i < 10 && !symbolReady; ++i) {
				signal.waitForSignal();
				reader.processMsgs();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			return symbolResults;
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
	public:
		void contractDetails(int reqId, const ContractDetails& details) override
		{
			this->details = details;
		}
		void contractDetailsEnd(int reqId) override 
		{
			reqId = reqId;
		}
		void req(const Contract& contract) {
			client.reqContractDetails(Id, contract);
		}
	};

	class HistoricalDataWrapper : public Wrapper {
	public:
		std::string headTimeStamp;
		HistoricalDataWrapper() = default;
		~HistoricalDataWrapper() = default;

		// Override EWrapper methods as needed
		void headTimestamp(int reqId, const std::string& headTimestamp) 
		{
			this->headTimeStamp = headTimestamp;
		}
		void historicalData(TickerId reqId, const Bar& bar) override
		{
			// Handle historical data here
		}
		void historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) override
		{
			//std::cout << "Historical data end." << std::endl;
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