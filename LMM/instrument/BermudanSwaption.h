#pragma once

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/instrument/CallableOption.h>
#include <vector>


class BermudanSwaption : public CallableOption
{
private:
	VanillaSwap              vanillaSwap_;        // The underlying swap, indexStart is the payment date.
	std::vector<VanillaSwap> livingSwapAtCallableDates_;
	
	virtual ~BermudanSwaption(){}
public:
	BermudanSwaption(const VanillaSwap& vanillaSwap, const std::vector<LMM::Index>& callableDatesIndex)
		:CallableOption(callableDatesIndex)
		 ,vanillaSwap_(vanillaSwap)
	{
		checkCallableDatesIndex();
		initialize_livingSwapAtCallableDates();
	}

	bool checkCallableDatesIndex();
	void initialize_livingSwapAtCallableDates();

	//! getter
	const std::vector<VanillaSwap>& get_livingSwapAtCallableDates_Ref() const {return livingSwapAtCallableDates_;}
	const VanillaSwap& get_vanillaSwap_Ref () const {return vanillaSwap_;}
};

typedef boost::shared_ptr<BermudanSwaption> BermudanSwaption_PTR;


