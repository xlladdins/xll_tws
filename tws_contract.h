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
	X(primaryExchange, std::string, String) \
	X(currency, std::string, String) \
	X(localSymbol, std::string, String) \
	X(tradingClass, std::string, String) \
	X(includeExpired, bool, Bool) \
	X(secIdType, std::string, String) \
	X(secId, std::string, String) \
	X(description, std::string , String) \
	X(issuerId, std::string, String) \
	X(comboLegsDescrip, std::string , String) \

#define TWS_CONTRACT_DETAILS(X) \
	X(std::string, marketName, String) \
	X(double, minTick, Num) \
	X(std::string, orderTypes, String) \
	X(std::string, validExchanges, String) \
	X(long, priceMagnifier, Long) \
	X(int, underConId, Int) \
	X(std::string, longName, String) \
	X(std::string, contractMonth, String) \
	X(std::string, industry, String) \
	X(std::string, category, String) \
	X(std::string, subcategory, String) \
	X(std::string, timeZoneId, String) \
	X(std::string, tradingHours, String) \
	X(std::string, liquidHours, String) \
	X(std::string, evRule, String) \
	X(double, evMultiplier, Num) \
	X(int, aggGroup, Int) \
	X(std::string, underSymbol, String) \
	X(std::string, underSecType, String) \
	X(std::string, marketRuleIds, String) \
	X(std::string, realExpirationDate, String) \
	X(std::string, lastTradeTime, String) \
	X(std::string, stockType, String) \
	X(Decimal, minSize, DecimalNum) \
	X(Decimal, sizeIncrement, DecimalNum) \
	X(Decimal, suggestedSizeIncrement, DecimalNum) \


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
