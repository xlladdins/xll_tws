// tws_error.h - TWS API error codes
#pragma once
#include <format>
#include <stdexcept>
#include "tws_datetime.h"

namespace tws {

    // ERR - unrecoverable error
	// WARNING - recoverable error
	// INFORMATION - informational message

    // System Errors
#define TWS_SYSTEM_ERRORS(X) \
    X(SYSTEM, 1100, INFORMATION, "Connectivity between IB and TWS has been lost.") \
    X(SYSTEM, 1101, INFORMATION, "Connectivity between IB and TWS has been restored - data maintained.") \
    X(SYSTEM, 1102, INFORMATION, "Connectivity between IB and TWS has been restored - data lost.") \
    X(SYSTEM, 1300, WARNING, "TWS socket port has been reset and this connection is being dropped. Please reconnect on the new port.")

    // Pacing Violations
#define TWS_PACING_VIOLATIONS(X) \
    X(PACING_VIOLATION, 10147, WARNING, "Max rate of messages per second has been exceeded.") \
    X(PACING_VIOLATION, 10148, WARNING, "Historical data request pacing violation.")

    // Order Errors
#define TWS_ORDER_ERRORS(X) \
    X(ORDER, 201, INFORMATION, "Order rejected.") \
    X(ORDER, 202, INFORMATION, "Order canceled.") \
    X(ORDER, 399, ERROR, "Order message error.") \
    X(ORDER, 404, INFORMATION, "Order not found.") \
    X(ORDER, 502, WARNING, "Couldn't connect to TWS. Confirm that \"Enable ActiveX and Socket Clients\" is enabled in TWS settings.") \
    X(ORDER, 503, ERROR, "The TWS is out of date and must be upgraded.")

    // Market Data Errors
#define TWS_MARKET_DATA_ERRORS(X) \
    X(MARKET_DATA, 2100, INFORMATION, "New account data requested from TWS.") \
    X(MARKET_DATA, 2101, WARNING, "Unable to subscribe to account data.") \
    X(MARKET_DATA, 2102, WARNING, "Unable to subscribe to account updates.") \
    X(MARKET_DATA, 2103, INFORMATION, "A market data farm is disconnected.") \
    X(MARKET_DATA, 2104, INFORMATION, "A market data farm is connected.") \
    X(MARKET_DATA, 2105, INFORMATION, "A historical data farm is disconnected.") \
    X(MARKET_DATA, 2106, INFORMATION, "A historical data farm is connected.") \
    X(MARKET_DATA, 2107, WARNING, "A historical data farm connection has become inactive but should be available upon demand.") \
    X(MARKET_DATA, 2108, WARNING, "A market data farm connection has become inactive but should be available upon demand.") \
    X(MARKET_DATA, 2110, INFORMATION, "Connectivity between IB and TWS has been restored.") \
    X(MARKET_DATA, 2119, INFORMATION, "Market depth data has been reset.")

    // Contract and Data Errors
#define TWS_CONTRACT_AND_DATA_ERRORS(X) \
    X(CONTRACT_AND_DATA, 200, WARNING, "No security definition has been found for the request.") \
    X(CONTRACT_AND_DATA, 300, WARNING, "Can't find EId with tickerId.") \
    X(CONTRACT_AND_DATA, 321, WARNING, "Error validating request.") \
    X(CONTRACT_AND_DATA, 322, WARNING, "Error validating request: Missing contract info.") \
    X(CONTRACT_AND_DATA, 323, WARNING, "Error validating request: Missing contract.") \
    X(CONTRACT_AND_DATA, 324, WARNING, "Error validating request: Missing tickerId.") \
    X(CONTRACT_AND_DATA, 325, WARNING, "Error validating request: Missing account code.") \
    X(CONTRACT_AND_DATA, 326, WARNING, "Error validating request: Missing action field.") \
    X(CONTRACT_AND_DATA, 327, WARNING, "Error validating request: Missing order type field.") \
    X(CONTRACT_AND_DATA, 328, WARNING, "Error validating request: Missing total quantity field.") \
    X(CONTRACT_AND_DATA, 329, WARNING, "Error validating request: Missing price field.")

    // Historical Data Errors
#define TWS_HISTORICAL_DATA_ERRORS(X) \
    X(HISTORICAL_DATA, 162, WARNING, "Historical Market Data Service error message.") \
    X(HISTORICAL_DATA, 165, WARNING, "HMDS query returned no data.")

// Miscellaneous Errors
#define TWS_MISCELLANEOUS_ERRORS(X) \
    X(MISCELLANEOUS, 100, WARNING, "Max number of tickers has been reached.") \
    X(MISCELLANEOUS, 101, WARNING, "Max number of historical data requests has been reached.") \
    X(MISCELLANEOUS, 102, WARNING, "Max number of scanner subscriptions has been reached.") \
    X(MISCELLANEOUS, 103, WARNING, "Max number of historical data requests has been reached.") \
    X(MISCELLANEOUS, 104, WARNING, "Max number of scanner subscriptions has been reached.") \
    X(MISCELLANEOUS, 105, WARNING, "Max number of historical data requests has been reached.") \
    X(MISCELLANEOUS, 106, WARNING, "Max number of scanner subscriptions has been reached.") \
    X(MISCELLANEOUS, 107, WARNING, "Max number of historical data requests has been reached.") \
    X(MISCELLANEOUS, 108, WARNING, "Max number of scanner subscriptions has been reached.") \
    X(MISCELLANEOUS, 109, WARNING, "Max number of historical data requests has been reached.") \
    X(MISCELLANEOUS, 110, WARNING, "Max number of scanner subscriptions has been reached.")

    // Financial Advisor Errors
#define TWS_FINANCIAL_ADVISOR_ERRORS(X) \
    X(FINANCIAL_ADVISOR, 503, ERROR, "The TWS is out of date and must be upgraded.") \
    X(FINANCIAL_ADVISOR, 504, WARNING, "Not connected.") \
    X(FINANCIAL_ADVISOR, 505, ERROR, "Fatal Unknown message id.") \
    X(FINANCIAL_ADVISOR, 506, ERROR, "Unsupported version.") \
    X(FINANCIAL_ADVISOR, 507, ERROR, "Bad message length.") \
    X(FINANCIAL_ADVISOR, 508, ERROR, "Bad message.") \
    X(FINANCIAL_ADVISOR, 509, ERROR, "Exception caught while reading socket.")

    enum class ErrorClass {
        SYSTEM,
        PACING_VIOLATION,
        ORDER,
        MARKET_DATA,
        CONTRACT_AND_DATA,
        HISTORICAL_DATA,
        MISCELLANEOUS,
        FINANCIAL_ADVISOR,
		UNKNOWN
    };

    enum class ErrorType {
        _ERROR,
        _WARNING,
        _INFORMATION,
        _UNKNOWN
    };

#define TWS_ERROR_TYPE(class, code, type, msg) if (errorCode == code) return ErrorType::##_##type;

    constexpr ErrorType errorType(int errorCode)
    {
        // Expand all error code macros
        TWS_SYSTEM_ERRORS(TWS_ERROR_TYPE)
        TWS_PACING_VIOLATIONS(TWS_ERROR_TYPE)
        TWS_ORDER_ERRORS(TWS_ERROR_TYPE)
        TWS_MARKET_DATA_ERRORS(TWS_ERROR_TYPE)
        TWS_CONTRACT_AND_DATA_ERRORS(TWS_ERROR_TYPE)
        TWS_HISTORICAL_DATA_ERRORS(TWS_ERROR_TYPE)
        TWS_MISCELLANEOUS_ERRORS(TWS_ERROR_TYPE)
        TWS_FINANCIAL_ADVISOR_ERRORS(TWS_ERROR_TYPE)

        return ErrorType::_UNKNOWN;
    }

    #undef TWS_ERROR_TYPE

    struct Error : public std::exception {
        int id;
        time_t errorTime;
        int errorCode;
        std::string errorString;
        std::string advancedOrderRejectJson;
        std::string what_;
        Error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson)
            : id(id), errorTime(errorTime), errorCode(errorCode), errorString(errorString), advancedOrderRejectJson(advancedOrderRejectJson)
        {
            what_ = std::format("id: {} time: {} error: {}", errorCode, DateTime(errorTime), errorString);
        }

        const char* what() const override
        {
            return what_.c_str();
        }
    };

 

} // namespace tws