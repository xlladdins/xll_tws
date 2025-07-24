// xll_tws.h - Trader Work Station API for Excel
#pragma once
#include <variant>
#pragma warning(disable: 4267)
#include "tws_api/EWrapper.h"
#include "tws_api/EClientSocket.h"
#include "tws_api/EReaderOSSignal.h"
#include "tws_api/EReader.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

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

	enum class ErrorType {
		Error,
		Warning,
		Info,
		Unknown
	};

	ErrorType Error(int errorCode) {
		// Error codes (partial list, expand as needed)
		if (100 <= errorCode && errorCode <= 999)
			return ErrorType::Error;
		// Warnings
		if (errorCode == 2104 || errorCode == 2106 || errorCode == 2107 || errorCode == 2108)
			return ErrorType::Warning;
		// Informational
		if (errorCode == 2103 || errorCode == 2105 || errorCode == 2158)
			return ErrorType::Info;
		// Add more codes as needed

		return ErrorType::Unknown;
	}

	// Value type to hold different tick data types
	using Value = std::variant<double, Decimal, std::string>;

	// Wrapper class to manage the connection and provide a default EWrapper implementation
	class Wrapper : public EWrapper {
	public:
		std::map<TickerId, std::map<TickType, Value>> tickData;
		OrderId orderId;
		EReaderOSSignal signal;
		EClientSocket client;
		Wrapper(const char* host = "127.0.0.1", int port = 7497, int clientId = 0, int timeout = 1000/*ms*/)
			: EWrapper(), orderId(0), signal(timeout), client(this, &signal)
		{
			if (!client.isConnected()) {
				client.setConnectOptions("+PACEAPI");
				ensure(client.eConnect(host, port, clientId));
			}
		}
		virtual ~Wrapper()
		{
			if (client.isConnected()) {
				client.eDisconnect();
			}
		}

		// reset???

		void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
		{
			char buffer[2048];
			sprintf_s(buffer, sizeof(buffer), "Error %d: %s (ID: %d, Time: %lld)", errorCode, errorString.c_str(), id, static_cast<long long>(errorTime));

			switch (Error(errorCode)) {
			case ErrorType::Error:
				XLL_ERROR(buffer);
				break;
			case ErrorType::Warning:
				XLL_WARNING(buffer);
				break;
			case ErrorType::Info:
				XLL_INFORMATION(buffer);
				break;
			default:
				XLL_ERROR(buffer);
			}
		}

		void nextValidId(OrderId orderId)
		{
			this->orderId = orderId;
		}
	};

	class HistoricalDataWrapper : public Wrapper {
	public:
		HistoricalDataWrapper()
			: Wrapper()
		{
		}
		~HistoricalDataWrapper() {}

		// Override EWrapper methods as needed
		void historicalData(TickerId reqId, const Bar& bar) override
		{
			// Handle historical data here
		}
		void historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) override
		{
			//std::cout << "Historical data end." << std::endl;
		}
		void nextValidId(OrderId orderId) override
		{
			//std::cout << "Connected. Next valid order id: " << orderId << std::endl;
		}
	};

	class MktDataWrapper : public Wrapper {
	public:

		MktDataWrapper()
			: Wrapper()
		{
		}
		~MktDataWrapper() {}

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

} // namespace tws