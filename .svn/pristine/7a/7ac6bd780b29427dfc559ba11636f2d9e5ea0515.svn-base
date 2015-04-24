#include <cassert>
#include <cmath>

#include <LMM/instrument/BermudanSwaption.h>


bool BermudanSwaption::checkCallableDatesIndex()
{
	LMM::Index startIndex = vanillaSwap_.get_indexStart();
	LMM::Index endIndex   = vanillaSwap_.get_indexEnd();

	for(size_t i=0; i<callableDatesIndex_.size(); ++i)
	{
		LMM::Index	callableIndex = callableDatesIndex_[i];
		if(callableIndex< startIndex || callableIndex>endIndex)
			return false;
	}	

	//! YY TODO: you should check also callableIndex is entire year, because of the constrain of the definition of swap !
	return true;
}

void BermudanSwaption::initialize_livingSwapAtCallableDates()
{
	 livingSwapAtCallableDates_.clear();
	 livingSwapAtCallableDates_.reserve(callableDatesIndex_.size());
	 for(size_t i=0; i<callableDatesIndex_.size(); ++i)
	 {
		 double strike = vanillaSwap_.get_strike();
		 LMM::Index indexStart = vanillaSwap_.get_indexStart();
		 LMM::Index indexEnd   = vanillaSwap_.get_indexEnd();
		 Tenor floatingLegTenorType = vanillaSwap_.get_floatingLegTenorType();
		 Tenor fixedLegTenorType = vanillaSwap_.get_fixedLegTenorType();
		 LMMTenorStructure_PTR lmmTenorStructure_ptr = vanillaSwap_.get_LMMTenorStructure();
		 
		 VanillaSwap vSwap(strike, indexStart,indexEnd, floatingLegTenorType, fixedLegTenorType, lmmTenorStructure_ptr);
		 livingSwapAtCallableDates_.push_back(vSwap);
	 }
}