#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>
#include <algorithm>
#include <LMM/helper/Name.h>


class CallableOption
{
protected:
	std::vector<LMM::Index> callableDatesIndex_;   // Suppose it's coincide with vanillaSwaption_'s floatingPaymentDatesIndex
	
public:
	CallableOption(const std::vector<LMM::Index>& callableDatesIndex)
		:callableDatesIndex_(callableDatesIndex)
	{
		// sort and remove duplication
		std::sort(callableDatesIndex_.begin(), callableDatesIndex_.end()); 
		auto last = std::unique(callableDatesIndex_.begin(), callableDatesIndex_.end());
		callableDatesIndex_.erase(last, callableDatesIndex_.end());
	}

	virtual ~CallableOption(){}

	//!getter
	const std::vector<LMM::Index>& get_callableDatesIndex_Ref() const {return callableDatesIndex_;}
};

typedef boost::shared_ptr<CallableOption> CallableOption_PTR;


