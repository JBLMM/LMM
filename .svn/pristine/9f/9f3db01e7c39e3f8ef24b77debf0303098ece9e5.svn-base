#include <LMM/pricer/McLmmBermudanSwaptionPricer.h>
#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>

void McLmmBermudanSwaptionPricer::preparePricerForInstrument(CallableOption_PTR callableOption_ptr)
{
	McLmmCallableOptionPricer::preparePricerForInstrument(callableOption_ptr); // call base class's version

	bermudanSwaption_ptr_ = boost::dynamic_pointer_cast<BermudanSwaption>(callableOption_ptr);
	assert(bermudanSwaption_ptr_!=0);

	initialize_EV();
	initialize_Basis();
}

void McLmmBermudanSwaptionPricer::initialize_EV()
{
	evsAtCallableDates_.clear();

	const std::vector<LMM::Index>& callableDatesIndex = bermudanSwaption_ptr_->get_callableDatesIndex_Ref();
	for(size_t itr_callableTimeIndex=0; itr_callableTimeIndex<callableDatesIndex.size(); ++itr_callableTimeIndex)
	{
		std::vector<EV_PTR> evs_ptr;
		evs_ptr.reserve(3);
		buffer_valueOfevsAtCallableD_ = std::vector<double>(3, 1.0e100);

		//! swapRate: 1st EV
		const VanillaSwap& vSwap = bermudanSwaption_ptr_->get_livingSwapAtCallableDates_Ref()[itr_callableTimeIndex];
		//McLmmVanillaSwapPricer_PTR mcLmmVSwap_Pricer_ptr =  McLmmVanillaSwapPricer_PTR(new McLmmVanillaSwapPricer(mcLmm_));
		EV_VanillaSwapRate_PTR ev_swap_ptr = EV_VanillaSwapRate_PTR(new EV_VanillaSwapRate(vSwap, mcLmmSwapPricer_ptr_));
		evs_ptr.push_back(ev_swap_ptr);

		//! liborRate: 2nd EV
		LMM::Index callableTimeIndex = callableDatesIndex[itr_callableTimeIndex];
		EV_LiborRate_PTR ev_frontLibor_ptr = EV_LiborRate_PTR(new EV_LiborRate(callableTimeIndex));
		evs_ptr.push_back(ev_frontLibor_ptr);

		//! swap payoff:  3rd EV
		// only for call not put !!!!! BUG !!!!!
	    //YY TODO: OptionType::OptionType vanillaSwaptionType_;  
		//YY TODO: this should be moved to swap class, for this reason cannot treat the case: CALL/put, but only call 
		double strike = bermudanSwaption_ptr_->get_vanillaSwap_Ref().get_strike();
		EV_payoffCall_PTR ev_payoffCall_ptr = EV_payoffCall_PTR(new EV_payoffCall(ev_swap_ptr,strike)); //YY ?? better to creat its own ev_Swap_PTR ???
		evs_ptr.push_back(ev_payoffCall_ptr);

		evsAtCallableDates_.push_back(evs_ptr);
	}	
}
void McLmmBermudanSwaptionPricer::initialize_Basis()
{
	basisAtCallableDates_.clear();

	const std::vector<LMM::Index>& callableDatesIndex = bermudanSwaption_ptr_->get_callableDatesIndex_Ref();
	for(size_t itr_callableTimeIndex=0; itr_callableTimeIndex<callableDatesIndex.size(); ++itr_callableTimeIndex)
	{
		PolynormialRegressionBasis_PTR polynomialBasis_ptr = PolynormialRegressionBasis_PTR(new PolynormialRegressionBasis(basisMonomialMaxOrder_, basisCorssnomialMaxOrder_));
		basisAtCallableDates_.push_back(polynomialBasis_ptr);
	}
}


void McLmmBermudanSwaptionPricer::bwdRegression()
{
	throw("Not finished yet: need create X and do regression ! ");
	matrix regression_X(nb_1stFwdSimulation_, basisAtCallableDates_[0]->get_nbBasis());  // size = (N,m), m = nbBasis
	matrix regression_Y(nb_1stFwdSimulation_,1); // size = (N,1)

	//! last callable Date
	const std::vector<LMM::Index>& callableDatesIndex = bermudanSwaption_ptr_->get_callableDatesIndex_Ref();
	LMM::Index lastCallableDIndex = callableDatesIndex.back();
	const VanillaSwap& lastAliveSwap = bermudanSwaption_ptr_->get_livingSwapAtCallableDates_Ref().back();
	for(size_t itr_simule=0; itr_simule<nb_1stFwdSimulation_; ++itr_simule)
	{
		double iv = mcLmmSwapPricer_ptr_->swapRate(lastCallableDIndex, 
			                                       lastAliveSwap,
												   numeraire_1stFwdSimulation_[itr_simule],
												   liborMatrix_1stFwdSimulation_[itr_simule]);
		double cv = 0.0;
		regression_Y(itr_simule,0) = std::max(iv,cv);	
	}
	// ---- do Regression here  ---- 

	//! backward 
	for(size_t itr_callableIndex=callableDatesIndex.size()-2; itr_callableIndex>=0; --itr_callableIndex)
	{
		LMM::Index callableDIndex_current = callableDatesIndex[itr_callableIndex];
		LMM::Index callableDIndex_next    = callableDatesIndex[itr_callableIndex+1];

		const VanillaSwap& aliveSwap = bermudanSwaption_ptr_->get_livingSwapAtCallableDates_Ref()[itr_callableIndex];

		//trj by trj update Y
		for(size_t itr_simule=0; itr_simule<nb_1stFwdSimulation_; ++itr_simule)
		{
			double cv = regression_Y(itr_simule,0)
				       *numeraire_1stFwdSimulation_[itr_simule][callableDIndex_current]
			           /numeraire_1stFwdSimulation_[itr_simule][callableDIndex_next];

			double iv =  mcLmmSwapPricer_ptr_->swapRate(callableDIndex_current, 
														aliveSwap,
														numeraire_1stFwdSimulation_[itr_simule],
														liborMatrix_1stFwdSimulation_[itr_simule]);
			regression_Y(itr_simule,0) = std::max(iv,cv);	
		}
	}
	//---- do Regression here  ---- 
}

//! evaluated by regression coeff
double McLmmBermudanSwaptionPricer::continuousValue(size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire)
{
	const std::vector<LMM::Index>& callableDatesIndex = bermudanSwaption_ptr_->get_callableDatesIndex_Ref();
	LMM::Index callableTimeIndex = callableDatesIndex[itr_callableTimeIndex];

	//! ev at callable date
	const std::vector<EV_PTR>& evAtCallableD = evsAtCallableDates_[itr_callableTimeIndex]; // YY does it do the copy ???
	for(size_t i=0; i<evAtCallableD.size(); ++i)
	{
		buffer_valueOfevsAtCallableD_[i] = evAtCallableD[i]->evaluate_ev(callableTimeIndex, liborMatrix, numeraire);
	}

	//! basis at callable date
	RegressionBasis_PTR basisAtCallableD = basisAtCallableDates_[itr_callableTimeIndex];
	basisAtCallableD->calculateBasisFromEV(buffer_valueOfevsAtCallableD_);
	const std::vector<double>& valueOfbasisAtCallableD = basisAtCallableD->get_basisRef();

	//! Y regressed = basis_emaple * coeff, coeff is saved in Regression[i]
	const Regression& regressionAtCallableD = regressions_[itr_callableTimeIndex];
	double Y_regressed = regressionAtCallableD.evaluateByRegression(valueOfbasisAtCallableD);
	return Y_regressed;
}

double McLmmBermudanSwaptionPricer::intrinsicValue (size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire)
{
	const std::vector<LMM::Index>& callableDatesIndex = bermudanSwaption_ptr_->get_callableDatesIndex_Ref();
	LMM::Index callableTimeIndex = callableDatesIndex[itr_callableTimeIndex];

	//! alive swap at callable date
	const VanillaSwap& aliveSwap = bermudanSwaption_ptr_->get_livingSwapAtCallableDates_Ref()[itr_callableTimeIndex];

    double aliveSwapRate = mcLmmSwapPricer_ptr_->swapRate(callableTimeIndex, aliveSwap, numeraire, liborMatrix);
	return aliveSwapRate;
}
