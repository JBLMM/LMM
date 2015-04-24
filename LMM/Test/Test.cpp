#include <LMM/Test/Tests.h>

#include <iostream>
#include <cassert>
#include <string.h>
#include <cmath>
#include <fstream>

#include <ql/termstructures/volatility/abcdcalibration.hpp>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/helper/InputFileManager.h>
#include <LMM/LmmModel/LmmSwaptionMarketData.h>
#include <LMM/helper/GenericPath.h>

#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>

#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <LMM/LmmModel/LmmABCDCalibrator.h>

#include <LMM/LmmModel/LmmCorrelationCostFunction.h>
#include <LMM/LmmModel/LmmCorrelationCalibrator.h>

#include <LMM/LmmModel/LmmBaseCalibrator.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>

#include <LMM/LmmModel/LmmGlobal_gCalibrator.h>
#include <LMM/LmmModel/LmmGlobal_gCostFunction.h>
#include <LMM/LmmModel/LmmLocal_gCalibrator.h>
#include <LMM/LmmModel/LmmLocal_gCostFunction.h>
#include <LMM/LmmModel/LmmCascade_gCalibrator.h>
#include <LMM/LmmModel/LmmCascade_gCostFunction.h>

LmmSwaptionMarketData_PTR get_LmmSwaptionMarketData(const LmmCalibrationConfig& config, const std::string& input_file)
{
	size_t nbYear = config.model_nbYear_;
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);

	return pLmmSwaptionMarketData;
}

LMMTenorStructure_PTR create___LMMTenorStructure_PTR(const size_t nbyear)
{
	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbyear) );
	return pLMMTenorStructure;
}

Correlation_PTR create_InitCorrelation(const LmmCalibrationConfig& config)
{
	size_t nbYear = config.model_nbYear_;

	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;

	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );

	size_t correlFullRank = config.correl_FullRank_;
	//size_t nbFactor       = (size_t) ( 0.6 * correlFullRank ) ; 
	size_t nbFactor       = config.correl_ReducedRank_ ; 
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;

	double init_correlAlpha =  config.correl_alpha_;
	double init_correlBeta  =  config.correl_beta_;

	Correlation_PTR pCorrelation( new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,init_correlAlpha,init_correlBeta) );
	pCorrelation->calculate(); 

	pCorrelation->print("init_correlation.csv");

	return pCorrelation;
}


QuantLib::Array marketData_LMM_ABCD_calibration(const LmmCalibrationConfig& config, LmmSwaptionMarketData_PTR pLmmSwaptionMarketData)
{
	std::string base_name = pLmmSwaptionMarketData->get_MarketDataBaseFileName() ;

	size_t nbYear = pLmmSwaptionMarketData->get_nbYear();
	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);

	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );


	ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, config.vol_abcd_ , 1., 0.));

	//! create correlation
	Correlation_PTR pCorrelation = create_InitCorrelation(config);

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, pCorrelation, pNoShifted_HGVolatilityParam)); 
	Dispersion dispersion(pVolatilityFunction);
	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	//! Create Approximation Rebonato 
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	LmmBaseCostFunction_PTR abcd_costFucntion (new LmmABCDCostFunction ( pLmmVanillaSwaptionApproxPricer_Rebonato
		, pLmmSwaptionMarketData->get_LiborQuotes()
		, pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()
		, pNoShifted_HGVolatilityParam
		));

	QuantLib::Array init_abcd(4);
	init_abcd[0]=config.vol_abcd_.a_;
	init_abcd[1]=config.vol_abcd_.b_;
	init_abcd[2]=config.vol_abcd_.c_;
	init_abcd[3]=config.vol_abcd_.d_;

	size_t maxIterations = 1000;
	size_t minStatIterations = 100;
	double rootEpsilon =1e-14;
	double functionEpsilon =1e-14;
	double gradientNormEpsilon =0;

	LmmABCDCalibrator  lmm_abcd_calibrator(init_abcd, maxIterations, rootEpsilon,functionEpsilon,abcd_costFucntion);


	//lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(1,1) );
	//lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(2,2) );
	////lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(3,3) );
	//lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(4,4) );
	//lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(6,6) );
	//lmm_abcd_calibrator.add_ConstraintCell(std::pair<size_t,size_t>(8,8) );

	lmm_abcd_calibrator.activate_PositiveConstraint();
	lmm_abcd_calibrator.solve();

	std::string result_file =base_name + "_abcd_calibration_result.csv";
	lmm_abcd_calibrator.printPlusPlus(result_file);

	std::string calibrated_abcd_file = base_name + "_abcd_calibrated.csv";
	pNoShifted_HGVolatilityParam->print(calibrated_abcd_file);

	// print in a common file
	{
		std::string common_result_file_name = "calib_result_ABCD.csv";
		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + common_result_file_name ;

		std::ofstream final_result ;
		final_result.open(full_common_result_file.c_str(), std::ios::app);

		final_result<<std::endl<<std::endl<< "============= Test At    "<<LMMPATH::get_TimeDateNow()
			<<",,,,,, Error LInf, "<<lmm_abcd_calibrator.get_QuoteError_LInf() <<std::endl ;
		final_result<< lmm_abcd_calibrator.get_BaseGeneral_Result_Info();

		final_result.close();	
	}

	return lmm_abcd_calibrator.get_Found_ABCD();
}


Correlation_PTR marketData_LMM_Correlation_calibration(const LmmCalibrationConfig& config, LmmSwaptionMarketData_PTR pLmmSwaptionMarketData , const QuantLib::Array& found_abcd)
{
	size_t nbYear = pLmmSwaptionMarketData->get_nbYear();
	std::string base_name = pLmmSwaptionMarketData->get_MarketDataBaseFileName() ;

	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);

	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );

	const double a=found_abcd[0],b=found_abcd[1],c=found_abcd[2],d=found_abcd[3];

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., 0.));

	//! create correlation
	Correlation_PTR pCorrelation = create_InitCorrelation(config);

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, pCorrelation, pNoShifted_HGVolatilityParam)); 
	Dispersion dispersion(pVolatilityFunction);
	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	//! Create Approximation Rebonato 
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	LmmBaseCostFunction_PTR pLmmCorrelationCostFunction(new LmmCorrelationCostFunction
		(
		pLmmVanillaSwaptionApproxPricer_Rebonato
		, pLmmSwaptionMarketData->get_LiborQuotes()
		, pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()
		, pCorrelation
		) );


	////// Correlation calibrator
	QuantLib::Array xy_correlation_init = pCorrelation->get_ArrayFrom_Correlation();
	QuantLib::Size maxIterations =1000;
	QuantLib::Size minStatIterations =100;
	QuantLib::Real rootEpsilon = 1e-8;
	QuantLib::Real functionEpsilon =  1e-8;


	LmmCorrelationCalibrator lmmCorrelationCalibrator
		(
		xy_correlation_init
		, maxIterations
		, rootEpsilon
		,functionEpsilon
		, pLmmCorrelationCostFunction
		);

	lmmCorrelationCalibrator.solve();

	std::string result_file =base_name + "_correlation_calibration_result.csv";
	lmmCorrelationCalibrator.printPlusPlus(result_file);

	std::string calibrated_correlation_file =base_name + "_correlation_calibrated.csv";
	Correlation_PTR calibrated_correl_ptr = lmmCorrelationCalibrator.get_Found_Correlation() ;
	calibrated_correl_ptr->print(calibrated_correlation_file);

	// print in a common file
	{
		std::string common_result_file_name = "calib_result_Correlation.csv";
		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + common_result_file_name ;

		std::ofstream final_result ;
		final_result.open(full_common_result_file.c_str(), std::ios::app);

		final_result<<std::endl<<std::endl<< "============= Test At    "<<LMMPATH::get_TimeDateNow()
			<<",,,,,, Error LInf, "<<lmmCorrelationCalibrator.get_QuoteError_LInf() <<std::endl ;
		final_result<< lmmCorrelationCalibrator.get_BaseGeneral_Result_Info();

		final_result.close();	
	}

	return calibrated_correl_ptr ;
}


GMatrixMapping_PTR marketData_LMM_CascadeExact_calibration( const LmmCalibrationConfig& config
														   , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
														   , const QuantLib::Array& abcd_param 
														   , Correlation_PTR found_correlation_ptr 
														   )
{
	size_t nbYear = pLmmSwaptionMarketData->get_nbYear();
	std::string base_file_name = pLmmSwaptionMarketData->get_MarketDataBaseFileName();

	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);

	std::string base_name=base_file_name +"_gMatrixCascadeCalibration" ;

	std::ostringstream file_lmm_mkt_stream;file_lmm_mkt_stream<<base_name<<"_Quotation_"<<nbYear<<"YR_.csv";
	pLmmSwaptionMarketData->print(file_lmm_mkt_stream.str());

	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );

	const double a=abcd_param[0];
	const double b=abcd_param[1];
	const double c=abcd_param[2];
	const double d=abcd_param[3];

	//const double a=0.0438867,b=0.0179444,c=0.554972,d=0.121429;/// ATTENTION< TEMPORARY, TODELETE and uses abcd calibrator above

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., 0.));

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, found_correlation_ptr, pNoShifted_HGVolatilityParam)); 
	Dispersion dispersion(pVolatilityFunction);
	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	// create gMatrixMapping
	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(pLMMTenorStructure->get_horizon() ,fixedfloatRatio );
	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	GMatrixMapping_PTR pGMatrixMapping( new GMatrixMapping(g_matrix_size, empty_delegate_matrix, pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()->get_UpperTriangularIndexPairMatrix())  );

	//initiate gMatrixMapping all gDelegate to 1
	QuantLib::Array g_delegate_vector =  pGMatrixMapping->get_DelegateArray();
	for(size_t i=0;i<g_delegate_vector.size();++i) g_delegate_vector[i] = 1.;
	pGMatrixMapping->reset_gDelegate(g_delegate_vector);

	pNoShifted_HGVolatilityParam->reset_g_matrix( pGMatrixMapping->get_g_Ref() );
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	LmmBaseCostFunction_PTR pLmmCascadeCostFunction (
		new LmmCascade_gCostFunction
		(
		pLmmVanillaSwaptionApproxPricer_Rebonato
		,pLmmSwaptionMarketData->get_LiborQuotes()
		,pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()
		,pGMatrixMapping
		,pNoShifted_HGVolatilityParam
		)
		);

	UpperTriangularDoubleMatrix copy_weight_matrix = pLmmCascadeCostFunction->get_SwaptionWeightMatrix();

	//copy_weight_matrix(2,1)=1000;

	pLmmCascadeCostFunction->reset_SwaptionWeightMatrix(copy_weight_matrix);

	LmmCascade_gCalibrator lmmCalibrator
		(
		*pGMatrixMapping.get()
		, 100000 // config.maxIter
		, 1e-12  // config.x_epsilon
		, 1e-10  // config.f_epsilon    
		, pLmmCascadeCostFunction
		);

	if(config.use_positive_constraint_)
		lmmCalibrator.activate_PositiveConstraint();

	lmmCalibrator.solve();

	std::ostringstream file_vol_stream;file_vol_stream<<base_name<<"_vol.csv";
	std::string file_calibrated_vol(file_vol_stream.str() );
	pNoShifted_HGVolatilityParam->print( file_calibrated_vol );

	std::ostringstream file_result_stream;file_result_stream<<base_name<<"_result.csv";
	std::string file_calibration_result(file_result_stream.str());
	lmmCalibrator.printPlusPlus(file_calibration_result);

	std::ostringstream file_gDelegate_stream;file_gDelegate_stream<<base_name<<"_gDelegate.csv";
	std::string file_gDelegate_vol(file_gDelegate_stream.str() );
	pGMatrixMapping->print(file_gDelegate_vol);

	// print in a common file
	{
		std::string common_result_file_name = "calib_result_gCascade.csv";
		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + common_result_file_name ;

		std::ofstream final_result ;
		final_result.open(full_common_result_file.c_str(), std::ios::app);

		final_result<<std::endl<<std::endl<< "============= Test At    "<<LMMPATH::get_TimeDateNow()
			<<",,,,,, Error LInf, "<<lmmCalibrator.get_QuoteError_LInf() <<std::endl ;
		final_result<< lmmCalibrator.get_BaseGeneral_Result_Info();

		final_result.close();	
	}

	return pGMatrixMapping;
}

void marketData_LMM_Global_gCalibration( const LmmCalibrationConfig& config
										, LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
										, const QuantLib::Array& abcd_param 
										, Correlation_PTR found_correlation_ptr  
										, GMatrixMapping_PTR init_gMapping 
										)
{
	assert(!config.use_local_calib_);
	size_t nbYear = pLmmSwaptionMarketData->get_nbYear();
	std::string base_file_name = pLmmSwaptionMarketData->get_MarketDataBaseFileName();

	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);

	std::string base_name;
	base_name = base_file_name+"_gMatrixGlobalCalibration" ;


	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );

	const double a=abcd_param[0];
	const double b=abcd_param[1];
	const double c=abcd_param[2];
	const double d=abcd_param[3];

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., 0.));

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, found_correlation_ptr , pNoShifted_HGVolatilityParam)); 
	Dispersion dispersion(pVolatilityFunction);
	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	// create gMatrixMapping
	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(pLMMTenorStructure->get_horizon() ,fixedfloatRatio );
	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	GMatrixMapping_PTR pGMatrixMapping;
	if(init_gMapping)
	{
		pGMatrixMapping = init_gMapping;
	}
	else
	{
		//initiate gMatrixMapping all gDelegate to 1
		pGMatrixMapping.reset( new GMatrixMapping(g_matrix_size, empty_delegate_matrix, pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()->get_UpperTriangularIndexPairMatrix())  );
		QuantLib::Array g_delegate_vector =  pGMatrixMapping->get_DelegateArray();
		for(size_t i=0;i<g_delegate_vector.size();++i) g_delegate_vector[i] = 1.;
		pGMatrixMapping->reset_gDelegate(g_delegate_vector);
	}

	pNoShifted_HGVolatilityParam->reset_g_matrix( pGMatrixMapping->get_g_Ref() );
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	// Create const function
	LmmPenalty_PTR pLmmPenalty(new LmmPenalty(config.penalty_time_homogeneity_,config.penalty_libor_) );

	LmmBaseCostFunction_PTR pLmmCostFunction(new LmmGlobal_gCostFunction
		(
		pLmmVanillaSwaptionApproxPricer_Rebonato,
		pLmmSwaptionMarketData->get_LiborQuotes(),
		pLmmSwaptionMarketData->get_SwaptionQuotes_ATM(),
		pGMatrixMapping,
		pNoShifted_HGVolatilityParam,
		pLmmPenalty
		) );

	//costumize swaptions weights
	UpperTriangularDoubleMatrix swpm_weight_matrix = pLmmCostFunction->get_SwaptionWeightMatrix();
	//swpm_weight_matrix(7,1)=1e-6;
	//swpm_weight_matrix(10,1)=1e-6;
	//swpm_weight_matrix(5,3)=0.;
	pLmmCostFunction->reset_SwaptionWeightMatrix(swpm_weight_matrix);

	//std::ostringstream file_costfunc_stream;file_costfunc_stream<<base_name<<"Calibration_"<<nbYear<<"YR_pel_time"<<penalty_time_homogene<<"_pel_lib"<<penalty_libor <<"_LmmCostFunction.csv";
	//pLmmCostFunction->print( file_costfunc_stream.str() );

	// Create Calibrator
	LmmGlobal_gCalibrator lmmCalibrator
		(
		*pGMatrixMapping.get()
		, 2000 //maxIter
		, 1e-11   //x_epsilon
		, 1e-11   //f_epsilon    
		, pLmmCostFunction
		);

	if(config.use_positive_constraint_)
		lmmCalibrator.activate_PositiveConstraint();

	lmmCalibrator.solve();

	std::string penalty_info_str;
	if( !pLmmPenalty->isEmpty() )
	{
		const double pT = config.penalty_time_homogeneity_;
		const double pL = config.penalty_libor_ ;
		std::ostringstream pel; pel<<"_pT_"<<pT<<"_pL_"<<pL;
		penalty_info_str = pel.str();
	}

	std::ostringstream file_result_stream;file_result_stream<<base_name<<penalty_info_str<<"_result.csv";
	std::string file_calibration_result(file_result_stream.str());
	lmmCalibrator.printPlusPlus(file_calibration_result);

	std::ostringstream file_gDelegate_stream;file_gDelegate_stream<<base_name<<penalty_info_str<<"_gDelegate.csv";
	std::string file_gDelegate_vol(file_gDelegate_stream.str() );
	pGMatrixMapping->print(file_gDelegate_vol);

	std::ostringstream file_vol_stream;file_vol_stream<<base_name<<penalty_info_str<<"_vol.csv";
	std::string file_calibrated_vol(file_vol_stream.str() );
	pNoShifted_HGVolatilityParam->print( file_calibrated_vol );


	config.result_quote_error_l2 = lmmCalibrator.get_QuoteError_L2();
	config.result_quote_error_l1  = lmmCalibrator.get_QuoteError_L1();
	config.result_quote_error_linf  = lmmCalibrator.get_QuoteError_LInf();

	if( !pLmmPenalty->isEmpty() )
	{
		config.result_pelTime_error_l2  = lmmCalibrator.get_PenaltyTimeHomogeneity_L2();
		config.result_pelTime_error_l1  = lmmCalibrator.get_PenaltyTimeHomogeneity_L1();
		config.result_pelTime_error_linf  = lmmCalibrator.get_PenaltyTimeHomogeneity_L_INF();
		config.result_pelLibor_error_l2  = lmmCalibrator.get_PenaltySmoothMaturity_L2();
		config.result_pelLibor_error_l1  = lmmCalibrator.get_PenaltySmoothMaturity_L1();
		config.result_pelLibor_error_linf  = lmmCalibrator.get_PenaltySmoothMaturity_L_INF();
	}

	{
		std::string common_result_file_name = "calib_result_gGlobal.csv";

		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + common_result_file_name ;

		std::ofstream final_result ;
		final_result.open(full_common_result_file.c_str(), std::ios::app);

		final_result<<std::endl<<std::endl<< "============= Test At    "<<LMMPATH::get_TimeDateNow()
			<<",,,,,, Error LInf, "<<lmmCalibrator.get_QuoteError_LInf() <<std::endl ;
		final_result<< lmmCalibrator.get_BaseGeneral_Result_Info();

		final_result.close();	
	}
}



void marketData_LMM_Local_gCalibration( const LmmCalibrationConfig& config
									   , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
									   , const QuantLib::Array& abcd_param 
									   , Correlation_PTR found_correlation_ptr  
									   , GMatrixMapping_PTR init_gMapping 
									   )
{
	assert(config.use_local_calib_);
	size_t nbYear = pLmmSwaptionMarketData->get_nbYear();
	std::string base_file_name = pLmmSwaptionMarketData->get_MarketDataBaseFileName();

	Tenor tenorfixedleg = Tenor::_1YR ;
	Tenor tenorfloatleg = Tenor::_6M  ;
	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);

	std::string base_name;
	base_name = base_file_name+"_gMatrixLocalCalibration" ;

	//create LMM components
	LMMTenorStructure_PTR pLMMTenorStructure( new LMMTenorStructure(tenorfloatleg,nbYear) );

	const double a=abcd_param[0];
	const double b=abcd_param[1];
	const double c=abcd_param[2];
	const double d=abcd_param[3];

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., 0.));

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, found_correlation_ptr , pNoShifted_HGVolatilityParam)); 
	Dispersion dispersion(pVolatilityFunction);
	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	// create gMatrixMapping
	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(pLMMTenorStructure->get_horizon() ,fixedfloatRatio );
	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	GMatrixMapping_PTR pGMatrixMapping;
	if(init_gMapping)
	{
		pGMatrixMapping = init_gMapping;
	}
	else
	{
		//initiate gMatrixMapping all gDelegate to 1
		pGMatrixMapping.reset( new GMatrixMapping(g_matrix_size, empty_delegate_matrix, pLmmSwaptionMarketData->get_SwaptionQuotes_ATM()->get_UpperTriangularIndexPairMatrix())  );
		QuantLib::Array g_delegate_vector =  pGMatrixMapping->get_DelegateArray();
		for(size_t i=0;i<g_delegate_vector.size();++i) g_delegate_vector[i] = 1.;
		pGMatrixMapping->reset_gDelegate(g_delegate_vector);
	}

	pNoShifted_HGVolatilityParam->reset_g_matrix( pGMatrixMapping->get_g_Ref() );
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	LmmBaseCostFunction_PTR pLmmCostFunction(new LmmLocal_gCostFunction
		(
		pLmmVanillaSwaptionApproxPricer_Rebonato,
		pLmmSwaptionMarketData->get_LiborQuotes(),
		pLmmSwaptionMarketData->get_SwaptionQuotes_ATM(),
		pGMatrixMapping,
		pNoShifted_HGVolatilityParam
		) );

	//costumize swaptions weights
	UpperTriangularDoubleMatrix swpm_weight_matrix = pLmmCostFunction->get_SwaptionWeightMatrix();
	//swpm_weight_matrix(7,1)=1e-6;
	//swpm_weight_matrix(10,1)=1e-6;
	//swpm_weight_matrix(5,3)=0.;
	pLmmCostFunction->reset_SwaptionWeightMatrix(swpm_weight_matrix);

	// Create Calibrator
	LmmLocal_gCalibrator lmmCalibrator
		(
		*pGMatrixMapping.get()
		, 3000 //maxIter
		, 1e-11   //x_epsilon
		, 1e-11   //f_epsilon    
		, pLmmCostFunction
		);

	if(config.use_positive_constraint_)
		lmmCalibrator.activate_PositiveConstraint();

	lmmCalibrator.solve();

	std::ostringstream file_result_stream;file_result_stream<<base_name<<"_result.csv";
	std::string file_calibration_result(file_result_stream.str());
	lmmCalibrator.printPlusPlus(file_calibration_result);

	std::ostringstream file_gDelegate_stream;file_gDelegate_stream<<base_name<<"_gDelegate.csv";
	std::string file_gDelegate_vol(file_gDelegate_stream.str() );
	pGMatrixMapping->print(file_gDelegate_vol);

	std::ostringstream file_vol_stream;file_vol_stream<<base_name<<"_vol.csv";
	std::string file_calibrated_vol(file_vol_stream.str() );
	pNoShifted_HGVolatilityParam->print( file_calibrated_vol );


	{
		std::string common_result_file_name = "calib_result_gLocal.csv";

		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + common_result_file_name ;

		std::ofstream final_result ;
		final_result.open(full_common_result_file.c_str(), std::ios::app);

		final_result<<std::endl<<std::endl<< "============= Test At    "<<LMMPATH::get_TimeDateNow()
			<<",,,,,, Error LInf, "<<lmmCalibrator.get_QuoteError_LInf() <<std::endl ;
		final_result<< lmmCalibrator.get_BaseGeneral_Result_Info();

		final_result.close();	
	}
}