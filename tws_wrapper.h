// tws_wrapper.h - Common EWrapper overrides
#pragma once
#include "tws_api/DefaultEWrapper.h"
#include "tws_api/EClientSocket.h"
#include "tws_api/EReaderOSSignal.h"
#include "tws_api/EReader.h"
#include "tws_error.h"

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
			throw tws::Error(id, errorTime, errorCode, errorString, advancedOrderRejectJson);
		}

	};


} // namespace tws
