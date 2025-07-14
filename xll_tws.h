// xll_tws.h - Trader Work Station API for Excel
#pragma once
#pragma warning(disable: 4267)
#include "tws_api/EWrapper.h"
#include "tws_api/EClientSocket.h"
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"TWS"
#endif

namespace tws {

	struct ClientSocket : public EClientSocket {
		ClientSocket(EWrapper* wrapper, EReaderSignal* signal = nullptr)
			: EClientSocket(wrapper, signal) 
		{}
		~ClientSocket() 
		{
			if (EClientSocket::isConnected()) {
				EClientSocket::eDisconnect();
			}
		}
		ClientSocket& connect(const char* host, int port, int clientId = 0, bool extraAuth = false)
		{
			if (!EClientSocket::eConnect(host, port, clientId, extraAuth)) {
				throw std::runtime_error("Failed to connect to TWS");
			}
			return *this;
		}
		operator EClientSocket&()	
		{
			return *this;
		}
	};

	class HistoricalDataWrapper : public EWrapper {
		ClientSocket client;
	public:
		HistoricalDataWrapper() 
			: EWrapper(), client(this)
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
		void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
		{
			//std::cerr << "Error: " << id << " " << code << " " << msg << std::endl;
		}
		
		HistoricalDataWrapper& connect(const char* host = "127.0.0.1", int port = 7496, int clientId = 0)
		{
			client.eConnect(host, port, clientId);

			return *this;
		}
	};


} // namespace tws