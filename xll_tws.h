// xll_tws.h - Trader Work Station API for Excel
#pragma once
#pragma warning(disable: 4267)
#include "tws_api/EWrapper.h"
#include "tws_api/EClientSocket.h"
#include "tws_api/EReaderOSSignal.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

namespace tws {

	// Wrapper class to manage the connection and provide a default EWrapper implementation
	class Wrapper : public EWrapper {
		EReaderOSSignal signal;
	public:
		EClientSocket client;
		Wrapper(const char* host = "", int port = 7497, int clientId = 0, int timeout = 1000/*ms*/)
			: EWrapper(), signal(timeout), client(this, &signal)
		{
			ensure(client.eConnect(host, port, clientId));
		}
		~Wrapper() 
		{ 
			if (client.isConnected()) {
				client.eDisconnect();
			}
		}

		void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
		{
			XLL_ERROR(errorString.c_str());
		}
	};

	class HistoricalDataWrapper : public Wrapper {
	public:
		HistoricalDataWrapper() 
			: Wrapper()
		{ }
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
		
		HistoricalDataWrapper& connect(const char* host = "127.0.0.1", int port = 7496, int clientId = 0)
		{
			client.eConnect(host, port, clientId);

			return *this;
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