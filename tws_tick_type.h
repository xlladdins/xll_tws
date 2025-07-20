// tws_tick_type.h - TWS Tick Type Definitions
/*
Use EDecoder::DecodeField???
*/
#pragma once
#include <map>

// https://interactivebrokers.github.io/tws-api/tick_types.html
// name, value, help, url
#define TWS_TICK_TYPE(X) \
X(BID_SIZE, 0, "Number of contracts or lots offered at the bid price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(BID, 1, "Highest priced bid for the contract.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ASK, 2, "Lowest price offer on the contract.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ASK_SIZE, 3, "Number of contracts or lots offered at the ask price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(LAST, 4, "Last price at which the contract traded (does not include some trades in RTVolume).", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LAST_SIZE, 5, "Number of contracts or lots traded at the last price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(HIGH, 6, "High price for the day.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LOW, 7, "Low price for the day.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(VOLUME, 8, "Trading volume for the day for the selected contract (US Stocks: multiplier 100).", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(CLOSE, 9, "The last available closing price for the previous day. For US Equities, we use corporate action processing to get the closing price, so the close price is adjusted to reflect forward and reverse splits and cash and stock dividends.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(BID_OPTION_COMPUTATION, 10, "Computed Greeks and implied volatility based on the underlying stock price and the option bid price. See Option Greeks", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#aaabfb00bd637027793c34387690b58d2") \
X(ASK_OPTION_COMPUTATION, 11, "Computed Greeks and implied volatility based on the underlying stock price and the option ask price. See Option Greeks", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#aaabfb00bd637027793c34387690b58d2") \
X(LAST_OPTION_COMPUTATION, 12, "Computed Greeks and implied volatility based on the underlying stock price and the option last traded price. See Option Greeks", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#aaabfb00bd637027793c34387690b58d2") \
X(MODEL_OPTION, 13, "Computed Greeks and implied volatility based on the underlying stock price and the option model price. Correspond to greeks shown in TWS. See Option Greeks", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#aaabfb00bd637027793c34387690b58d2") \
X(LOW_13_WEEK, 15, "Lowest price for the last 13 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(HIGH_13_WEEK, 16, "Highest price for the last 13 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LOW_26_WEEK, 17, "Lowest price for the last 26 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(HIGH_26_WEEK, 18, "Highest price for the last 26 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LOW_52_WEEK, 19, "Lowest price for the last 52 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(HIGH_52_WEEK, 20, "Highest price for the last 52 weeks. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(AVG_VOLUME, 21, "The average daily trading volume over 90 days. Multiplier of 100. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(OPEN_INTEREST, 22, "(Deprecated, not currently in use) Total number of options that are not closed.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(OPTION_HISTORICAL_VOL, 23, "The 30-day historical volatility (currently for stocks).", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(OPTION_IMPLIED_VOL, 24, "A prediction of how volatile an underlying will be in the future.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(OPTION_BID_EXCH, 25, "Not Used.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(OPTION_ASK_EXCH, 26, "Not Used.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(OPTION_CALL_OPEN_INTEREST, 27, "Call option open interest.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(OPTION_PUT_OPEN_INTEREST, 28, "Put option open interest.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(OPTION_CALL_VOLUME, 29, "Call option volume for the trading day.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(OPTION_PUT_VOLUME, 30, "Put option volume for the trading day.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(INDEX_FUTURE_PREMIUM, 31, "The number of points that the index is over the cash index.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(BID_EXCH, 32, "For stock and options, identifies the exchange(s) posting the bid price. See Component Exchanges", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(ASK_EXCH, 33, "For stock and options, identifies the exchange(s) posting the ask price. See Component Exchanges", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(AUCTION_VOLUME, 34, "The number of shares that would trade if no new orders were received and the auction were held now.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(AUCTION_PRICE, 35, "The price at which the auction would occur if no new orders were received and the auction were held now- the indicative price for the auction. Typically received after Auction imbalance (tick type 36)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(AUCTION_IMBALANCE, 36, "The number of unmatched shares for the next auction; returns how many more shares are on one side of the auction than the other. Typically received after Auction Volume (tick type 34)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(MARK_PRICE, 37, "The mark price is the current theoretical calculated value of an instrument. Since it is a calculated value, it will typically have many digits of precision.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(BID_EFP_COMPUTATION, 38, "Computed EFP bid price", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(ASK_EFP_COMPUTATION, 39, "Computed EFP ask price", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(LAST_EFP_COMPUTATION, 40, "Computed EFP last price", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(OPEN_EFP_COMPUTATION, 41, "Computed EFP open price", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(HIGH_EFP_COMPUTATION, 42, "Computed high EFP traded price for the day", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(LOW_EFP_COMPUTATION, 43, "Computed low EFP traded price for the day", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(CLOSE_EFP_COMPUTATION, 44, "Computed closing EFP price for previous day", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#af8afed0428f2f848f15ac137d81283fc") \
X(LAST_TIMESTAMP, 45, "Time of the last trade (in UNIX time).", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(SHORTABLE, 46, "Describes the level of difficulty with which the contract can be sold short. See Shortable", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(RT_VOLUME, 48, "Last trade details (Including both 'Last' and 'Unreportable Last' trades). See RT Volume", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(HALTED, 49, "Indicates if a contract is halted. See Halted", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(BID_YIELD, 50, "Implied yield of the bond if it is purchased at the current bid.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ASK_YIELD, 51, "Implied yield of the bond if it is purchased at the current ask.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LAST_YIELD, 52, "Implied yield of the bond if it is purchased at the last price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(CUST_OPTION_COMPUTATION, 53, "Greek values are based off a user customized price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#aaabfb00bd637027793c34387690b58d2") \
X(TRADE_COUNT, 54, "Trade count for the day.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(TRADE_RATE, 55, "Trade count per minute.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(VOLUME_RATE, 56, "Volume per minute.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(LAST_RTH_TRADE, 57, "Last Regular Trading Hours traded price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(RT_HISTORICAL_VOL, 58, "30-day real time historical volatility.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(IB_DIVIDENDS, 59, "Contract's dividends. See IB Dividends.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(BOND_FACTOR_MULTIPLIER, 60, "The bond factor is a number that indicates the ratio of the current bond principal to the original principal", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(REGULATORY_IMBALANCE, 61, "The imbalance that is used to determine which at-the-open or at-the-close orders can be entered following the publishing of the regulatory imbalance.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(NEWS_TICK, 62, "Contract's news feed.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(SHORT_TERM_VOLUME_3_MIN, 63, "The past three minutes volume. Interpolation may be applied. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(SHORT_TERM_VOLUME_5_MIN, 64, "The past five minutes volume. Interpolation may be applied. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(SHORT_TERM_VOLUME_10_MIN, 65, "The past ten minutes volume. Interpolation may be applied. For stocks only.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_BID, 66, "Delayed bid price. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_ASK, 67, "Delayed ask price. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_LAST, 68, "Delayed last traded price. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_BID_SIZE, 69, "Delayed bid size. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_ASK_SIZE, 70, "Delayed ask size. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_LAST_SIZE, 71, "Delayed last size. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_HIGH, 72, "Delayed highest price of the day. See Market Data Types.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_LOW, 73, "Delayed lowest price of the day. See Market Data Types", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_VOLUME, 74, "Delayed traded volume of the day. See Market Data Types", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_CLOSE, 75, "The prior day's closing price.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_OPEN, 76, "Not currently available", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(RT_TRD_VOLUME, 77, "Last trade details that excludes 'Unreportable Trades'. See RT Trade Volume", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(CREDITMAN_MARK_PRICE, 78, "Not currently available", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(CREDITMAN_SLOW_MARK_PRICE, 79, "Slower mark price update used in system calculations", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_BID_OPTION_COMPUTATION, 80, "Computed greeks based on delayed bid price. See Market Data Types and Option Greeks.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_ASK_OPTION_COMPUTATION, 81, "Computed greeks based on delayed ask price. See Market Data Types and Option Greeks.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_LAST_OPTION_COMPUTATION, 82, "Computed greeks based on delayed last price. See Market Data Types and Option Greeks.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(DELAYED_MODEL_OPTION_COMPUTATION, 83, "Computed Greeks and model's implied volatility based on delayed stock and option prices.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(LAST_EXCH, 84, "Exchange of last traded price", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(LAST_REG_TIME, 85, "Timestamp (in Unix ms time) of last trade returned with regulatory snapshot", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(FUTURES_OPEN_INTEREST, 86, "Total number of outstanding futures contracts (TWS v965+). *HSI open interest requested with generic tick 101", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(AVG_OPT_VOLUME, 87, "Average volume of the corresponding option contracts(TWS Build 970+ is required)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(DELAYED_LAST_TIMESTAMP, 88, "Delayed time of the last trade (in UNIX time) (TWS Build 970+ is required)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a19cb7c5bbd4ab05ccc5f9e686ed07a9e") \
X(SHORTABLE_SHARES, 89, "Number of shares available to short (TWS Build 974+ is required)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a5ac52bbd35c189e4ed684185da869e02") \
X(ETF_NAV_CLOSE, 92, "Today's closing price of ETF's Net Asset Value (NAV). Calculation is based on prices of ETF's underlying securities.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_PRIOR_CLOSE, 93, "Yesterday's closing price of ETF's Net Asset Value (NAV). Calculation is based on prices of ETF's underlying securities.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_BID, 94, "The bid price of ETF's Net Asset Value (NAV). Calculation is based on prices of ETF's underlying securities.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_ASK, 95, "The ask price of ETF's Net Asset Value (NAV). Calculation is based on prices of ETF's underlying securities.", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_LAST, 96, "The last price of Net Asset Value (NAV). For ETFs: Calculation is based on prices of ETF's underlying securities. For NextShares: Value is provided by NASDAQ", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_FROZEN_NAV_LAST, 97, "ETF Nav Last for Frozen data", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_HIGH, 98, "The high price of ETF's Net Asset Value (NAV)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ETF_NAV_LOW, 99, "The low price of ETF's Net Asset Value (NAV)", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#ae851ec3a1e0fa2d0964c7779b0c89718") \
X(ESTIMATED_IPO_MIDPOINT, 101, "Midpoint is calculated based on IPO price range", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \
X(FINAL_IPO_LAST, 102, "Final price for IPO", "https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html#a9901f14408b7a6f75b1d06e3e6196a26") \

namespace tws {

#define TWS_TICK_TYPE_MAP(name, value, help, url) { #name, value },
	inline std::map<std::string, int> TickType = {
			TWS_TICK_TYPE(TWS_TICK_TYPE_MAP)
	};

} // namespace tws