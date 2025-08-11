// tws_mkt_data.h
#pragma once
#include "tws_wrapper.h"

namespace tws {


	class MktDataWrapper : public Wrapper {
	public:
		MktDataWrapper()
			: Wrapper()
		{
		}
		~MktDataWrapper() {}
		/*
		void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib& attrib) override
		{
			tickData[tickerId][field] = price;
		}
		void tickSize(TickerId tickerId, TickType field, Decimal size) override
		{
			tickData[tickerId][field] = size;
		}
		void tickString(TickerId tickerId, TickType tickType, const std::string& value) override
		{
			tickData[tickerId][tickType] = value;
		}
		void tickGeneric(TickerId tickerId, TickType tickType, double value) override
		{
			tickData[tickerId][tickType] = value;
		}
		*/
	};

} // namespace tws
