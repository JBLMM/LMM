#include <LMM/Test/Tests.h>

#include <iostream>
#include <cassert>
#include <string.h>
#include <cmath>
#include <fstream>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>
#include <cmath>

// ---- include for QuantLib calibration -------
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/bfgs.hpp> 
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
// ---- include for QuantLib calibration -------

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/numeric/NumericalMethods.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/helper/Noise.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>
#include <LMM/calibration/SwaptionMarketDataManager.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/calibration/CalibrationShiftCostFunction.h>
//#include <LMM/calibration/TestHGVolatilityParam.h>

LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(const Tenor& tenorfixedleg,const Tenor& tenorfloatleg,LMMTenorStructure_PTR pLmmTenorStructure, const double const_shift );
ConstShifted_HGVolatilityParam_PTR createConstShifted_HGVolatilityParam(LMMTenorStructure_PTR, const double const_shift);
QuantLib::Array perturbe(const QuantLib::Array& init_param, const double & noise);

void printFinalResultToFile(const std::string & basename, 
							const QuantLib::Array & init_full_param,  
							const QuantLib::Array &  perturbed_full_param,  
							const QuantLib::Array &  calibrated_full_param,
							const SwaptionMarketDataContainer_PTR mkt_data,
							const LmmVanillaSwaptionApproxPricer_Rebonato_PTR approx							
							);

void test_CalibrationShiftWithSwaptionVolSkew()
{ 
	std::cout<<"test_CalibrationShiftWithSwaptionVolSkew() called "<<std::endl;

	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;

	size_t nbYear = 6;//nbYear has to be greater than 5 in order to have nbshift <nbSwaptions

	const double strike_bump = 0.001;
	const double const_rate  = 0.02;
	const double const_shift = 0.01;
	const double perturbe_noise = 0.1;

	
	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer = createLmmVanillaSwaptionApproxPricer(tenorfixedleg,tenorfloatleg,pLmmTenorStructure,const_shift );

	SwaptionMarketDataManager swaption_market_manager( pLmmTenorStructure ) ;

	SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer_skew = swaption_market_manager.generateSkewData(tenorfixedleg, tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),const_rate,strike_bump);
		
	LmmVanillaSwapPricer_PTR swap_pricer_ptr ( new LmmVanillaSwapPricer(pLmmTenorStructure) );
	assert( pSwaptionMarketDataContainer_skew->check_data_consistance() );
	assert( pSwaptionMarketDataContainer_skew->is_ATMSwaptionData(swap_pricer_ptr) );	
	pSwaptionMarketDataContainer_skew->print("test_CalibrationShiftWithSwaptionVolSkew_Data.csv");

	ConstShifted_HGVolatilityParam_PTR volParams_ptr = createConstShifted_HGVolatilityParam(pLmmTenorStructure,const_shift);
	
	volParams_ptr->print("test_CalibrationShiftWithSwaptionVolSkew_initShift.csv");
	const QuantLib::Array init_full_param = volParams_ptr->get_ArrayFrom_Shift();
	const QuantLib::Array perturbed_full_param = perturbe(init_full_param, perturbe_noise);

	volParams_ptr->reset_Shift_FromArray(perturbed_full_param);
	volParams_ptr->print("test_CalibrationShiftWithSwaptionVolSkew_perturbedShift.csv");
	pLmmApproxVanillaSwaptionPricer->update_VolatilityParam(volParams_ptr);


	CalibrationShiftCostFunction myFunc(volParams_ptr,pSwaptionMarketDataContainer_skew,pLmmApproxVanillaSwaptionPricer);
	myFunc.reset_CalibReference(init_full_param);

	QuantLib::PositiveConstraint constraint ;
	
	QuantLib::Array init_calib(perturbed_full_param);
	QuantLib::Problem myProb ( myFunc , constraint , init_calib );

	QuantLib::Size maxIterations       = 5000 ;
	QuantLib::Size minStatIterations   = 30  ;
	QuantLib::Real rootEpsilon         = 1e-12; 
	QuantLib::Real functionEpsilon     = 1e-15; 
	QuantLib::Real gradientNormEpsilon = 0.   ;//1e-7 ;
	QuantLib::EndCriteria myEndCrit ( maxIterations ,minStatIterations ,rootEpsilon ,functionEpsilon ,gradientNormEpsilon );

	QuantLib::LevenbergMarquardt solver(functionEpsilon,rootEpsilon,1e-16) ;
	QuantLib::EndCriteria::Type solvedCrit = solver.minimize(myProb,myEndCrit);

	const QuantLib::Array calibrated_full_param = myProb.currentValue();//volParams_ptr->get_ArrayFrom_Shift();
	volParams_ptr->print("test_CalibrationShiftWithSwaptionVolSkew_calibratedShift.csv");

	printFinalResultToFile("test_CalibrationShiftWithSwaptionVolSkew_", 
						   init_full_param,  
						   perturbed_full_param,  
						   calibrated_full_param,
						   pSwaptionMarketDataContainer_skew,
						   pLmmApproxVanillaSwaptionPricer							
							);
}


LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(const Tenor& tenorfixedleg,const Tenor& tenorfloatleg,LMMTenorStructure_PTR pLmmTenorStructure, const double const_shift )
{
	// =================   CREATION OF LMM_PTR   =============================
	ConstShifted_HGVolatilityParam_PTR volParams_ptr = createConstShifted_HGVolatilityParam(pLmmTenorStructure,const_shift);

	const unsigned int nbVolParams = volParams_ptr->get_nbCalibrateParam();
	QuantLib::Array init_volparam(nbVolParams, 1.0, 0.); // vector 1. everywhere

	// =================  END  CREATION OF LMM_PTR   =============================
	//! Correlation 
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	//Correlation_PTR correlation(new Const_Correlation(correlFullRank,correlReducedRank, correlReductionType,0.)); //not work
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.

	//Lmm
	//double shiftValue = 0.0; // -0.01;
	//std::vector<double> shifts(pLmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.04;
	std::vector<double> liborsInitValue(pLmmTenorStructure->get_horizon()+1, fwdRate);

	ConstShifted_HGVolatilityFunction_PTR hgVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLmmTenorStructure, correlation, volParams_ptr)); 

	//! Dispersion
	Dispersion dispersion(hgVolatilityFunction);

	Lmm_PTR lmm_ptr(new Lmm(dispersion));

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));					 

	return pLmmApproxVanillaSwaptionPricer;
}

ConstShifted_HGVolatilityParam_PTR createConstShifted_HGVolatilityParam(LMMTenorStructure_PTR pLmmTenorStructure, const double const_shift)
{
	// =================   CREATION OF LMM_PTR   =============================
	//init abcd default a=-0.06,  b=0.17, c=0.54, d=0.17
	double a=0.22,b=0.4,c=0.6,d=0.15;

	double g_constParam = 1.0;
	double shift_constParam = const_shift;

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR volParams_ptr( new ConstShifted_HGVolatilityParam(pLmmTenorStructure, abcdParam, g_constParam, shift_constParam));

	return volParams_ptr;
}

QuantLib::Array perturbe(const QuantLib::Array& init_param, const double & noise)
{
	unsigned int tempo_seed = NoiseHelper::get_Seed();
	
	QuantLib::Array perturbed(init_param);	
	
	std::vector<double> multiplicative_noise = Multiplicative::UniformNoise(init_param.size(),noise, tempo_seed);

	for(size_t i=0;i<perturbed.size();++i)
	{
		perturbed[i]*= multiplicative_noise[i]; 
	}

	return perturbed;
}

void printFinalResultToFile(const std::string & basename, 
							const QuantLib::Array & init_full_param,  
							const QuantLib::Array &  perturbed_full_param,  
							const QuantLib::Array &  calibrated_full_param,
							const SwaptionMarketDataContainer_PTR mkt_data,
							const LmmVanillaSwaptionApproxPricer_Rebonato_PTR approx							
							)
{

	size_t nbFullParam = init_full_param.size();

	std::ostringstream file_compar_mkt_approx;file_compar_mkt_approx << LMMPATH::get_output_path() << basename <<"_Results.csv";
	std::ofstream result_stream;
	result_stream.open(file_compar_mkt_approx.str());

	result_stream<<std::endl<< " -------COMPARISON INIT SHIFT vs CALIBRATED SFHIT--------,"<<std::endl
		<<std::endl<< "InitShift , PerturbedShift,	CalibratedShift, ,diffPerturb,  diffCalib,"<<std::endl;
	double l2_diff_calib=0,l2diff_perturb=0,linf_diff_calib=0,linf_diff_perturb=0;
	for(size_t i=0;i<nbFullParam;++i)
	{
		result_stream<<init_full_param[i]<<"              ,  "<<perturbed_full_param[i]<<"           ,  "<<calibrated_full_param[i]<<",,";
		double diffperturb = std::abs(perturbed_full_param[i] - init_full_param[i]);
		double diffcalib = std::abs( calibrated_full_param[i]- init_full_param[i]);
		result_stream<<diffperturb<<"," <<diffcalib<<",";
		if(diffperturb > diffcalib)
			result_stream<<"Ok,";
		else
			result_stream<<"Bad,";
		result_stream<<std::endl;

		l2_diff_calib+=diffcalib*diffcalib;
		l2diff_perturb+=diffperturb*diffperturb;
		if(linf_diff_calib<diffcalib) linf_diff_calib=diffcalib;
		if(linf_diff_perturb<diffperturb) linf_diff_perturb=diffperturb;
	}
	result_stream<<std::endl;
	result_stream<< ",,,L2, "<<l2diff_perturb<<","<<l2_diff_calib <<std::endl;
	result_stream<< ",,,L Inf, "<<linf_diff_perturb<<","<<linf_diff_calib <<std::endl;

	const SwaptionMarketDataContainer::RealVector &      marketInitLIBORs = mkt_data->get_LIBOR_INIT();
	const SwaptionMarketDataContainer::SwaptionVector &  marketSwaptions  = mkt_data->get_SWAPTION_VECTOR();
	const SwaptionMarketDataContainer::RealVector &      marketSkews       = mkt_data->get_VOLATILITY_VECTOR();
	const double  &      strike_bump       = mkt_data->get_StrikeBump();

	size_t nbSwaption = marketSwaptions.size();

	std::vector<double> approxSkews;
	for (const auto& swaption : marketSwaptions)
	{
		VanillaSwaption copy_swaption(swaption);
		double approxSkew = approx->skew(copy_swaption ,marketInitLIBORs, strike_bump);
		approxSkews.push_back( approxSkew );
	}

	result_stream<<std::endl<< " -------COMPARISON MARKET's SKEW vs APPROXIMATION CALIBRATED SKEW --------,"<<std::endl;
	result_stream<< "Market ,"<<"	Approx,"<<std::endl;
	double L2_diff=0.;
	double Linf_diff=0.;
	for(size_t i=0;i<nbSwaption;++i)
	{
		result_stream<< marketSkews[i]<<",	"<< approxSkews[i]<<","<<std::endl;
		L2_diff += std::abs(marketSkews[i]*marketSkews[i] - approxSkews[i]*approxSkews[i]);
		if(Linf_diff < std::abs(marketSkews[i] - approxSkews[i]) )
			Linf_diff= std::abs(marketSkews[i] - approxSkews[i]);
	}
	result_stream<< ",,L2 normalized diff Approx vs Market , "<<std::sqrt(L2_diff)/nbSwaption<<","<<std::endl;
	result_stream<< ",,LInf normalized diff Approx vs Market , "<<Linf_diff<<","<<std::endl;

	result_stream<< "########,########,########,########,########,"<<std::endl<<std::endl<<std::endl;

	result_stream.close();
}