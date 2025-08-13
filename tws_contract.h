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
	X(marketName, std::string, String) \
	X(minTick, double, Num) \
	X(orderTypes, std::string, String) \
	X(validExchanges, std::string, String) \
	X(priceMagnifier, long, Long) \
	X(underConId, int, Int) \
	X(longName, std::string, String) \
	X(contractMonth, std::string, String) \
	X(industry, std::string, String) \
	X(category, std::string, String) \
	X(subcategory, std::string, String) \
	X(timeZoneId, std::string, String) \
	X(tradingHours, std::string, String) \
	X(liquidHours, std::string, String) \
	X(evRule, std::string, String) \
	X(evMultiplier, double, Num) \
	X(aggGroup, int, Int) \
	X(underSymbol, std::string, String) \
	X(underSecType, std::string, String) \
	X(marketRuleIds, std::string, String) \
	X(realExpirationDate, std::string, String) \
	X(lastTradeTime, std::string, String) \
	X(stockType, std::string, String) \
	X(minSize, Decimal, DecimalNum) \
	X(sizeIncrement, Decimal, DecimalNum) \
	X(suggestedSizeIncrement, Decimal, DecimalNum) \
// TODO: more fields

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
