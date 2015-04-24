#include <LMM/pricer/LmmVanillaSwapPricer.h>


LmmVanillaSwapPricer::LmmVanillaSwapPricer(LMMTenorStructure_PTR lmmTenorStructure)	: pLMMTenorStructure_(lmmTenorStructure){};

LmmVanillaSwapPricer::~LmmVanillaSwapPricer()
{}

double LmmVanillaSwapPricer::swapNPV_Analytical_1(const VanillaSwap& vanillaSwap, const std::vector<double>& liborsInitValue)  const // initLibor[i] = L_i[T_0]
{
	assert(pLMMTenorStructure_->get_horizon()+1 == liborsInitValue.size());
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
	size_t horizon = pLMMTenorStructure_->get_horizon();


	const double & floatingLegdelta_T = vanillaSwap.get_floatingLegTenorType().YearFraction();
	const double & fixedLegdelta_T = vanillaSwap.get_fixedLegTenorType().YearFraction();

	//! ZC[i] = P(T_0,T_i)
	std::vector<double> ZC(horizon+2);
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		ZC[i] = ZC[i-1]/(1+floatingLegdelta_T*liborsInitValue[i-1]);
	}

	//! pvFloatingLeg: 1 - 1
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	//size_t floatingLegTenorLmmTenorRatio = vanillaSwap.get_floatingLegTenorLmmTenorRatio();
	LMM::Index indexFloatingLegStart = floatingLegPaymentIndexSchedule.front();
	LMM::Index indexFloatingLegEnd   = floatingLegPaymentIndexSchedule.back();
	double pvFloatingLeg =  ZC[indexFloatingLegStart-1] - ZC[indexFloatingLegEnd];

	////! pvFloatingLeg: exact
	//double pvFloatingLeg = 0.0;
	//const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	//for(size_t itr = 0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)
	//{
	//	//! At time T_{i+1}, pay: L_i(T_i)
	//	size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // = i+1
	//	size_t indexLibor			   = floatingLegPaymentIndex-1; // =i, because : floatingTenor = lmmTenor  
	//	pvFloatingLeg				  += floatingLegdelta_T*initLibor[indexLibor]*ZC[floatingLegPaymentIndex];		
	//}

	//! pvFixedLeg
	double pvFixedLeg = 0.0;
	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
	{
		size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; // = i+1
		pvFixedLeg 		    	   += fixedLegdelta_T*ZC[fixedLegPaymentIndex];		
	}
	pvFixedLeg *= vanillaSwap.get_strike();

	return pvFloatingLeg - pvFixedLeg;
}

double LmmVanillaSwapPricer::swapNPV_Analytical_2(const VanillaSwap& vanillaSwap, const std::vector<double>& liborsInitValue)  const // initLibor[i] = L_i[T_0]
{
	assert(pLMMTenorStructure_->get_horizon()+1 == liborsInitValue.size());
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
	size_t horizon = pLMMTenorStructure_->get_horizon();


	const double & floatingLegdelta_T = vanillaSwap.get_floatingLegTenorType().YearFraction();
	const double & fixedLegdelta_T = vanillaSwap.get_fixedLegTenorType().YearFraction();

	//! ZC[i] = P(T_0,T_i)
	std::vector<double> ZC(horizon+2);
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		ZC[i] = ZC[i-1]/(1+floatingLegdelta_T*liborsInitValue[i-1]);
	}

	//! numeraire
	std::vector<double> numeraire(ZC.size() ); // determinisitc IR
	for(size_t i=0; i<ZC.size(); ++i)
	{
		numeraire[i] = 1.0/ZC[i];
	}

	//! pvFloatingLeg: 1 - 1
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	//size_t floatingLegTenorLmmTenorRatio = vanillaSwap.get_floatingLegTenorLmmTenorRatio();
	LMM::Index indexFloatingLegStart = floatingLegPaymentIndexSchedule.front();
	LMM::Index indexFloatingLegEnd   = floatingLegPaymentIndexSchedule.back();
	double pvFloatingLegValue =  ZC[indexFloatingLegStart-1] - ZC[indexFloatingLegEnd];


	////! pvFloatingLeg: exact
	//double pvFloatingLeg = 0.0;
	//const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	//for(size_t itr = 0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)
	//{
	//	//! At time T_{i+1}, pay: L_i(T_i)
	//	size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // = i+1
	//	size_t indexLibor			   = floatingLegPaymentIndex-1; // =i, because : floatingTenor = lmmTenor  
	//	pvFloatingLeg				  += floatingLegdelta_T*initLibor[indexLibor]*ZC[floatingLegPaymentIndex];		
	//}


	LMM::Index indexValuationDate = 0;
	double pvFixedLegValue = pvFixedLeg(indexValuationDate,vanillaSwap,numeraire);
	return pvFloatingLegValue - pvFixedLegValue;
}


double LmmVanillaSwapPricer::swapRate_Analytical(const VanillaSwap& vanillaSwap,
											  const std::vector<double>& liborsInitValue)  const
{
	assert(pLMMTenorStructure_->get_horizon()+1 == liborsInitValue.size());
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
	size_t horizon = pLMMTenorStructure_->get_horizon();
	
	const double & fixedLegdelta_T = vanillaSwap.get_fixedLegTenorType().YearFraction();
	const double & floatingLegdelta_T = vanillaSwap.get_floatingLegTenorType().YearFraction();

	//! ZC[i] = P(T_0,T_i)
	std::vector<double> ZC(vanillaSwap.get_indexEnd()+1); 
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		size_t indexLibor = i-1;
		//double deltaT = vanillaSwap.get_DeltaTFloatLeg(indexLibor); // YY bug 
		double deltaT = floatingLegdelta_T; 
		ZC[i] = ZC[i-1]/(1+deltaT*liborsInitValue[indexLibor]);
	}

	//! pvFloatingLeg: 1 - 1
	double pvFloatingLegValue =  ZC[vanillaSwap.get_indexStart()] - ZC[vanillaSwap.get_indexEnd()];

	//! pvAnnuity
	double pvAnnuity = annuity0(vanillaSwap, liborsInitValue);

	//! swapRate
	return pvFloatingLegValue / pvAnnuity;
}



double LmmVanillaSwapPricer::annuity(  
	const LMM::Index            indexValuationDate,
	const VanillaSwap    & vanillaSwap,
	const std::vector<double> & numeraire) const
{
	assert(indexValuationDate <= vanillaSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;

	double price = 0.0;
	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
	{
		size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; 
		double delta_T              = vanillaSwap.get_DeltaTFixedLeg(itr);
		//std::cout << "numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] = " << numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] << std::endl;
		price			           += delta_T*numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex];		
	}
	return price;
}

//! Annuity at time 0 
double LmmVanillaSwapPricer::annuity0(const VanillaSwap& vanillaSwap,
								   const std::vector<double>& liborsInitValue)  const
{
	assert(pLMMTenorStructure_->get_horizon()+1 == liborsInitValue.size());
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
	size_t horizon = pLMMTenorStructure_->get_horizon();

	const double & fixedLegdelta_T = vanillaSwap.get_fixedLegTenorType().YearFraction();
	const double & floatingLegdelta_T = vanillaSwap.get_floatingLegTenorType().YearFraction();

	//! ZC[i] = P(T_0,T_i)
	std::vector<double> ZC(vanillaSwap.get_indexEnd()+1); 
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		size_t indexLibor = i-1;
		//double deltaT = vanillaSwap.get_DeltaTFloatLeg(indexLibor); // YY bug
		double deltaT = floatingLegdelta_T;
		ZC[i] = ZC[i-1]/(1+deltaT*liborsInitValue[indexLibor]);
	}

	double price = 0.0;
	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
	{
		size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; 
		double delta_T              = vanillaSwap.get_DeltaTFixedLeg(itr);
		//std::cout << "numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] = " << numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] << std::endl;
		price			           += delta_T*ZC[fixedLegPaymentIndex];
	}
	return price;
}

//! what a shit and terrible function.
double LmmVanillaSwapPricer::omega0(LMM::Index i, const double& annuity_T0, const std::vector<double>& bonds_T0) const
{
	throw("what a shit code ! make sure it doesn't contains a bug.");
	//double per = (tenorDates_[i+1] - tenorDates_[i]);
	const double& per = pLMMTenorStructure_->get_deltaT(i);
	double res = per * bonds_T0[i+1];
	return res/annuity_T0;
}


double LmmVanillaSwapPricer::forwardSwapRate( 
	const LMM::Index            indexValuationDate,
	const VanillaSwap    & vanillaSwap,
	const std::vector<double> & numeraire) const
{
	throw("Error, dangerous to make a function returns zero!");

	/* What are you doing ? 
	// see Damiano Brigo 2006 p238
	const size_t T_alpha = vanillaSwap.get_indexStart();
	const size_t T_beta = vanillaSwap.get_indexEnd();

	assert(indexValuationDate <= T_alpha); 
	assert(pLMMTenorStructure_->get_horizon() >= T_beta);  
		 
	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();

	const double C_alpha_beta = this->annuity(indexValuationDate,vanillaSwap,numeraire);
	const size_t alphaIndex = indexValuationDate;
	const size_t betaIndex = fixedLegPaymentIndexSchedule.back();
	const double P_t_alpha = 1.0/ numeraire[alphaIndex];
	const double P_t_beta  = 1.0/ numeraire[betaIndex];

	double S_alpha_beta = (P_t_alpha - P_t_beta) / C_alpha_beta;

	return S_alpha_beta;
	*/
	return 0.0;
}

double LmmVanillaSwapPricer::pvFixedLeg(
	const LMM::Index            indexValuationDate,
	const VanillaSwap   & vanillaSwap,
	const std::vector<double> & numeraire)  const
{
	assert(indexValuationDate <= vanillaSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation
	assert(pLMMTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
	const double calculatedAnuity = this->annuity(indexValuationDate,vanillaSwap,numeraire);
	return vanillaSwap.get_strike()*calculatedAnuity;
}
