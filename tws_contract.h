// tws_contract.h
#pragma once
#include <string>
#include "tws_matching_symbols.h"

// name, tws type, Excel type
#define TWS_CONTRACT(X) \
	X(conId, long, Long) \
	X(symbol, std::string, String) \
	X(secType, std::string, String) \
	X(lastTradeDateOrContractMonth, std::string, String) \
	X(lastTradeDate, std::string , String) \
	X(strike, double, Num) \
	X(right, std::string, String) \
	X(multiplier, std::string, String) \
	X(exchange, std::string, String) \
	X(primaryExchange , std::string, String) \
	X(currency, std::string, String) \
	X(localSymbol, std::string, String) \
	X(tradingClass, std::string, String) \
	X(includeExpired, bool, Bool) \
	X(secIdType, std::string, String) \
	X(secId, std::string, String) \
	X(description, std::string , String) \
	X(issuerId, std::string, String) \
	X(comboLegsDescrip, std::string , String) \

namespace tws {

	// https://interactivebrokers.github.io/tws-api/basic_contracts.html
	// https://ibkrcampus.com/campus/ibkr-api-page/twsapi-doc/#contracts
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
