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


} // namespace tws