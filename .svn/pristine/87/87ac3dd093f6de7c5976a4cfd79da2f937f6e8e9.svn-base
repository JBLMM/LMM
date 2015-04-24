#include <cassert>
#include <vector>

#include <LMM/numeric/NumericalMethods.h>
#include <LMM/pricer/LmmApproxVanillaSwaptionPricer.h>

LmmApproxVanillaSwaptionPricer::LmmApproxVanillaSwaptionPricer(const Lmm_PTR& lmmModel)
	: VanillaSwapPricer(lmmModel->get_LMMTenorStructure() )
	, lmm_(lmmModel)
	, buffer_ZC(lmm_->get_horizon()+2)
	, buffer_numeraire(lmm_->get_horizon()+2)
	, buffer_omega()
	, buffer_anuity()
{
	assert(lmm_->get_LMMTenorStructure()->get_tenorDate()[0] == 0.0);
}

LmmApproxVanillaSwaptionPricer::~LmmApproxVanillaSwaptionPricer()
{

}

void LmmApproxVanillaSwaptionPricer::reset_VolatilityParam(const VolatilityParam& vol_param) 
{
	lmm_->reset_VolatilityParam(vol_param);
}


void LmmApproxVanillaSwaptionPricer::preCalculateNumeraireAndZC(const std::vector<double> & liborsInitValue) const
{
	assert(liborsInitValue.size() + 1 == buffer_ZC.size() );

	const std::vector<double>& deltaT = lmm_->get_LMMTenorStructure()->get_deltaT();

	buffer_ZC[0] = 1.0;buffer_numeraire[0]=1.0;
	for(size_t i=1; i<buffer_ZC.size(); ++i)
	{
		size_t indexLibor = i-1;
		// P(0,T_{i-1}) / P(0,T_i)   = 1 + (T_{i}-T_{i-1}) L(0,T_{i-1},T_i)
		buffer_ZC[i] = buffer_ZC[i-1]/(1+deltaT[indexLibor]*liborsInitValue[indexLibor]);
		buffer_numeraire[i] = 1/buffer_ZC[i];
	}
}

void LmmApproxVanillaSwaptionPricer::preCalculateAnuity(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	const LMM::VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();

	buffer_anuity = annuity( 0, vanillaSwap, buffer_numeraire);
}


void LmmApproxVanillaSwaptionPricer::preCalculateOmega(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	assert(liborsInitValue.size() + 1 == buffer_ZC.size() );

	const LMM::VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();

	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();

	buffer_omega.clear();
	for(size_t itr=0; itr<floatingLegPaymentIndexSchedule.size(); ++itr)
	{
		//! At time T_{i+1}, pay: delta_t*L_i(T_i)
		size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // = i+1
		//size_t indexLibor = floatingLegPaymentIndex-1; // =i, because : floatingTenor = lmmTenor  
		//size_t indexT     = indexLibor;                                        // = i
		const std::vector<double>& deltaTFloatingLeg_ = vanillaSwap.get_DeltaTFloatLeg();
		const double& delta_T    = deltaTFloatingLeg_[itr];  
		buffer_omega.push_back( delta_T*buffer_ZC[floatingLegPaymentIndex] / buffer_anuity ); 
	}
}

void LmmApproxVanillaSwaptionPricer::preCalculateALL (const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	this->preCalculateNumeraireAndZC(liborsInitValue) ;
	this->preCalculateAnuity        (liborsInitValue, vanillaswaption) ;
	this->preCalculateOmega         (liborsInitValue, vanillaswaption) ;
}

double LmmApproxVanillaSwaptionPricer::price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const 
{
	throw ("Bug, Need to change!");
	this->preCalculateALL(libor_init  ,swaption);

	const LMM::VanillaSwap & swap = swaption.getUnderlyingSwap();

	double swaprate0  = this->swapRate_Analytical(swap, libor_init) ;
	double strike     = swap.get_strike() ;
	double vol_approx = this->volBlack(swaption,libor_init) ;
	
	LMM::Index swaptionIndexMaturity = swaption.get_indexMaturity();
	const double & timeToMaturity = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);

	double approxPrice = NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , timeToMaturity) ;
	//double approxPrice = NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , buffer_anuity)  ; //older code

	return approxPrice;
}


double LmmApproxVanillaSwaptionPricer::volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const
{
	this->preCalculateALL(liborsInitValue  ,vanillaswaption);

	const LMM::VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();

	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_simulationTenorType() );

	//! Robonato Formula: YY TODO: can be simplified: use the symmetric ! 
	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();

	double volSquare = 0.0;
	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
	{
		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
		//size_t indexT_i = indexLibor_i;
		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
		{
			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = j+1
			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;

			//size_t indexT_j     = indexLibor_j;
			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]

			const double& cumulated_covariance_tensor = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);
			volSquare += buffer_omega[i]*buffer_omega[j]*liborsInitValue[indexLibor_i]*liborsInitValue[indexLibor_j]*cumulated_covariance_tensor; 
		}
	}

	double swapRate = swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0
	double volBlack = std::sqrt(volSquare) / swapRate;

	return volBlack;
}


void LmmApproxVanillaSwaptionPricer::accumulateShiftedSwapRateAndStrike(
	double& out_shifted_swapRate_T0 ,
	double& out_shifted_strike      ,
	const std::vector<double> & omega0         ,
	const std::vector<double> & libor_shifts   ,
	const std::vector<size_t> & floatingIndices)const
{
	//-- Compute shifted swap rate and shifted strike 
	double swap_shift = 0.0;

	for (auto i : floatingIndices)
		swap_shift +=  omega0[i-1]*libor_shifts[i-1]; //-- Check the index

	out_shifted_swapRate_T0 += swap_shift;
	out_shifted_strike += swap_shift;	
}

//YY: TODO: Wrong formula,  to check why 
double LmmApproxVanillaSwaptionPricer::computeRebonatoVolatility(
	const VanillaSwaption     & swaption    ,
	const std::vector<double> & libor       ,
	const std::vector<double> & libor_shift ,
	const std::vector<double> & omega       ,
	const double & shifted_swapRate_T0) const
{
	//throw("Error: this function is buggy, should not be used, but should not replace it simply by the function volBalck(), because the shift is not actived.");
	//Throw of Yuan Li break the code 

	size_t swaptionIndexMaturity = swaption.get_indexMaturity();
	const LMM::VanillaSwap & swap = swaption.getUnderlyingSwap() ;
	const std::vector<size_t> & floatingIndices = swap.get_floatingLegPaymentIndexSchedule()     ;
	const std::vector<double> & tenorDates      = lmm_->get_LMMTenorStructure()->get_tenorDate() ;

	double vol_squared = 0.;
	// Adrien 2013  p.31 p.32
	for(auto floatIndex_i : floatingIndices) // = i+1
	{
		size_t indexLibor_i = floatIndex_i - 1;
		//size_t indexT_i = indexLibor_i;

		for(auto floatIndex_j : floatingIndices )// = j+1
		{
			size_t indexLibor_j = floatIndex_j - 1;
			//size_t indexT_j     = indexLibor_j;
			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]

			const double& covInteg_T_ij = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);

			double omega_ij = omega[indexLibor_i]*omega[indexLibor_j];
			double libor_ij = (libor[indexLibor_i]+libor_shift[indexLibor_i])*(libor[indexLibor_j]+libor_shift[indexLibor_j]);
			vol_squared += omega_ij * libor_ij * covInteg_T_ij; 
		}
	}

	vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);	

	return vol_squared;
}


//LmmApproxSwaptionPricer::SwaptionApproximation(const SwaptionApproximation& approximation)
//{
//	throw("Error: copy cosntructor no implememted");
//	//swapStartDateIndex_ = approximation.swapStartDateIndex_;
//	//swapEndDateIndex_ = approximation.swapEndDateIndex_;
//	//vol_ = approximation.vol_;
//	//tenorDates_ = approximation.tenorDates_;
//}
//

//
//
//double LmmApproxSwaptionPricer::computeRebonatoVolatility(size_t swapStartDateIndex, 		                                        
//	                                            double shifted_swapRate_T0,
//	                                            double annuity_T0, 
//												const std::vector<size_t>& floatingIndices,
//												const std::vector<double>& bonds_T0,
//												const std::vector<double>& libors_T0,
//												const std::vector<double>& libor_shifts)
//{
//	double T_maturity = tenorDates_[swapStartDateIndex];
//	//-- Compute vol under swap measure
//	double vol_squared = 0.;
//
//	for each (size_t i in floatingIndices) 
//	{
//		double Ti = tenorDates_[i-1];
//		for each (size_t j in floatingIndices)
//		{
//			double Tj = tenorDates_[j-1];
//			double volTmp = vol_->covIntegral(i-1,j-1,Ti,Tj,0,T_maturity);
//			vol_squared += omega0(i-1,annuity_T0,bonds_T0) * omega0(j-1,annuity_T0,bonds_T0) * (libors_T0[i-1]+libor_shifts[i-1]) * (libors_T0[j-1] +libor_shifts[j-1])* volTmp;
//		}
//	}
//
//	vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);
//
//	return vol_squared;
//}
//
//
//
////-- Squared volatility from Rebonato's approx is an out parameter
//double LmmApproxSwaptionPricer::RebonatoApprox(size_t swapStartDateIndex,
//	                                 double strike,
//									 const std::vector<size_t>& floatingIndices,
//	                                 double& out_vol_squared,
//	                                 double swapRate_T0, 
//									 double annuity_T0, 
//									 const std::vector<double>& bonds_T0,
//									 const std::vector<double>& libors_T0,
//									 const std::vector<double>& libor_shifts)
//{ 
//	
//	//double strike = (vanillaSwaption.getUnderlyingSwap()).get_strike();
//	
//	//-- In case swap starts at time 0
//	if (swapStartDateIndex == 0) 
//	{
//		return annuity_T0*std::max(swapRate_T0-strike,0.);
//	}
//	else 
//	{
//		//-- Compute shifted swap rate and shifted strike 
//		double swap_shift = 0.0;
//		for each (size_t i in floatingIndices)
//			swap_shift +=  omega0(i-1,annuity_T0,bonds_T0)*libor_shifts[i-1]; //-- Check the index
//		
//		double shifted_swapRate_T0 = swapRate_T0 + swap_shift;
//		double shifted_strike = strike + swap_shift;
//
//		//-- Compute vol under swap measure
//		double T_maturity = tenorDates_[swapStartDateIndex];
//		out_vol_squared = 0.;
//		for each (size_t i in floatingIndices) 
//		{
//			double Ti = tenorDates_[i-1];
//			for each (size_t j in floatingIndices)
//			{
//				double Tj = tenorDates_[j-1];
//				double volTmp = vol_->covIntegral(i-1,j-1,Ti,Tj,0,T_maturity);
//				out_vol_squared += omega0(i-1,annuity_T0,bonds_T0) * omega0(j-1,annuity_T0,bonds_T0) * (libors_T0[i-1]+libor_shifts[i-1]) * (libors_T0[j-1] +libor_shifts[j-1])* volTmp;
//			}
//		}
//
//		out_vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);
//		
//		//-- Apply Black formula
//		NumericalMethods blackCapletPricer;
//		double price = blackCapletPricer.BlackFormula(shifted_swapRate_T0,shifted_strike,out_vol_squared,annuity_T0);
//
//		return price;
//	}
//}
//
//


//
//double LmmApproxSwaptionPricer::AndersenApprox() {return 0.;}
//
//
//const std::vector<double>& LmmApproxSwaptionPricer::getTenorDates() {return tenorDates_;}

//double LmmApproxVanillaSwaptionPricer::Rebonato_SwaptionPrice( 
//	const double& swapRate_T0, 
//	const double& strike, 
//	const double& vol_squared, 
//	const double& annuity) const 
//{
//	return NumericalMethods::Black_Price(swapRate_T0,strike,vol_squared,annuity);
//}
