// tws_contract_details.h - Contract details API for TWS
#pragma once
#include "tws_wrapper.h"

namespace tws {

	class ContractDetailsWrapper : public Wrapper {
		ContractDetails details;
		bool done = false;
	public:
		void contractDetails(int reqId, const ContractDetails& details) override
		{
			this->details = details;
		}
		void contractDetailsEnd(int reqId) override
		{
			reqId = reqId;
			done = true;
		}
		void reset()
		{
			done = false;
		}
		void req(const Contract& contract) {
			reset();
			connect();
			EReader reader(&client, &signal);
			reader.start();
			client.reqContractDetails(Id(), contract);
			while (!done) {
				signal.waitForSignal();
				reader.processMsgs();
			}
		}
	};

} // namespace tws