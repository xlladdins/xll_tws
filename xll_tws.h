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

	class ClientSocket : public EClientSocket {
		std::unique_ptr<EReaderOSSignal> signal;
	public:
		ClientSocket(EWrapper* wrapper, int timeout = 1000)
			: signal(std::make_unique<EReaderOSSignal>(timeout)),
			EClientSocket(wrapper, signal.get())
		{ }
		~ClientSocket()
		{
			if (EClientSocket::isConnected()) {
				EClientSocket::eDisconnect();
			}
		}
		operator EClientSocket&()	
		{
			return *this;
		}
	};

	// Wrapper class to manage the connection and provide a default EWrapper implementation
	class Wrapper : public EWrapper {
	protected:
		ClientSocket client;
	public:
		Wrapper(const char* host = "", int port = 7496, int clientId = 0, int timeout = 1000)
			: EWrapper(), client(this, timeout)
		{
			client.eConnect(host, port, clientId);
		}
		~Wrapper() 
		{ }
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
			XLL_ERROR(errorString.c_str());
		}
		
		HistoricalDataWrapper& connect(const char* host = "127.0.0.1", int port = 7496, int clientId = 0)
		{
			client.eConnect(host, port, clientId);

			return *this;
		}
	};


} // namespace tws