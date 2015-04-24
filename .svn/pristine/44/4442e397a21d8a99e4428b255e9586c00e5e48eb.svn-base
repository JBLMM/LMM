#pragma once
#include <boost/shared_ptr.hpp>

#include <LMM/LmmModel/McLmm.h>        // Model
#include <LMM/instrument/BermudanSwaption.h>  // Instrument
#include <LMM/pricer/McLmmVanillaSwapPricer.h>
#include <LMM/pricer/McLmmCallableOptionPricer.h>
#include <vector>
#include <LMM/pricer/Regression.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>
/*
//! YY TODO: ugly class
//1. move to LmmVanillaSwapPricer
//2. check if lmm->lmmMatrix and vanillaSwap has the same floatingTenor, it's important for pricing
//3. impose condition: assert(vanillaSwap.get_indexStart() == indexValutionDate)
class McLmmBermudanSwaptionPricer_Helper_VanillaSwapPricer
{
public:
	double swapRateAtEvaluationDate(const LMM::Index   indexValutionDate,
		                            const VanillaSwap& vanillaSwap,
									const matrix&      liborMatrix)
	{
		assert(vanillaSwap.get_indexStart() == indexValutionDate); // this simplify the pricing
		assert(liborMatrix.size1() > vanillaSwap.get_indexEnd());  // can do the pricing

		double floatingLegDelta_T = vanillaSwap.get_floatingLegTenorType().YearFraction();
		double fixedLegDelta_T    = vanillaSwap.get_fixedLegTenorType().YearFraction();

		//ZC_atSwapStartDate[i] = P(T_s,T_{s+i}), T_s = swapStartDate
		std::vector<double> ZC_atSwapStartDate (vanillaSwap.get_indexEnd() - vanillaSwap.get_indexStart() +1, 1.0e100);
		ZC_atSwapStartDate[0] = 1.0;
		for(size_t i=0; i<ZC_atSwapStartDate.size(); ++i)
		{
			LMM::Index liborIndex = indexValutionDate + i -1;
			ZC_atSwapStartDate[i] = ZC_atSwapStartDate[i-1]/(1+floatingLegDelta_T*liborMatrix(liborIndex,indexValutionDate));
		}

		//! floatingPV
		double floatingPV = ZC_atSwapStartDate[vanillaSwap.get_indexStart()] - ZC_atSwapStartDate[vanillaSwap.get_indexEnd()];
			
		//! annuity 
		double annuity = 0.0;
		const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule();
		for(size_t itr=0; itr<fixedLegPaymentIndexSchedule.size(); ++itr)
		{
			size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr];
			annuity += fixedLegDelta_T*ZC_atSwapStartDate[fixedLegPaymentIndex-indexValutionDate];
		}
		return floatingPV/annuity;
	}
};
typedef boost::shared_ptr<McLmmBermudanSwaptionPricer_Helper_VanillaSwapPricer> McLmmBermudanSwaptionPricer_Helper_VanillaSwapPricer_PTR;
*/

class McLmmBermudanSwaptionPricer : public McLmmCallableOptionPricer // to reuse the already implemented code in VanillaSwapPricer.
{
	BermudanSwaption_PTR bermudanSwaption_ptr_;
	size_t basisMonomialMaxOrder_;    // basis
	size_t basisCorssnomialMaxOrder_; // basis

	std::vector<std::vector<EV_PTR>> evsAtCallableDates_;    // ith element correspond to callableDatesIndex[i]
	std::vector<RegressionBasis_PTR> basisAtCallableDates_;  // ith element correspond to callableDatesIndex[i]

	std::vector<double> buffer_valueOfevsAtCallableD_;    // evs' value at one callableDate
	McLmmVanillaSwapPricer_PTR mcLmmSwapPricer_ptr_;      // used to price vanillaSwap (EV and intrinsic value)
														  // because don't have the visitor design pattern, and cannot creat automatically the price.

public:
	McLmmBermudanSwaptionPricer(McLmm_PTR mcLmm_ptr,
								size_t nb_1stFwdSimulation,
								size_t nb_2ndFwdSimulation,
								size_t basisMonomialMaxOrder,    // basis param
								size_t basisCorssnomialMaxOrder) // basis param
		: McLmmCallableOptionPricer(mcLmm_ptr, nb_1stFwdSimulation, nb_2ndFwdSimulation)
		, basisMonomialMaxOrder_(basisMonomialMaxOrder)
	    , basisCorssnomialMaxOrder_(basisCorssnomialMaxOrder)
	{
		assert(basisCorssnomialMaxOrder_ == 2); //YY for the moment: only work for this case
		mcLmmSwapPricer_ptr_ = McLmmVanillaSwapPricer_PTR(new McLmmVanillaSwapPricer(mcLmm_ptr));
	}

	virtual ~McLmmBermudanSwaptionPricer(){};

	virtual void preparePricerForInstrument(CallableOption_PTR callableOption_ptr); 
	void initialize_EV();
	void initialize_Basis();

	virtual void bwdRegression();      

	virtual double continuousValue(size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire); // evaluated by regression coeff
	virtual double intrinsicValue (size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire);
};

typedef boost::shared_ptr<McLmmBermudanSwaptionPricer> McLmmBermudanSwaptionPricer_PTR;