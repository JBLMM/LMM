#include <LMM/Test/Tests.h>
#include <LMM/Test/Test_CalibrationConfig.h>

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
#include <LMM/helper/LmmGnuplotPrinterMatrix.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>
#include <LMM/calibration/SwaptionMarketDataManager.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/LmmSkewCostFunction.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>
#include <LMM/LmmModel/LmmBaseCalibrator.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/LmmModel/LmmSwaptionMarketData.h>
#include <LMM/LmmModel/LmmShiftCalibrator.h>

#include <LMM/LmmModel/LmmGlobal_gCalibrator.h>
#include <LMM/LmmModel/LmmLocal_gCalibrator.h>
#include <LMM/LmmModel/LmmCascade_gCalibrator.h>

void test_LmmVirtual_gCalibration(const Test_CalibrationConfig& config);
//void test_LmmVirtualCalibrationShift(const Test_CalibrationConfig& config);

void test_LmmVirtualCalibration()
{
	//std::cout<<"test_LmmVirtualCalibration() called"<<std::endl;

	Test_CalibrationConfig config; // always re_init before each test

	config.want_print_full_vol_init   =true;
	config.want_print_full_vol_perturb=true;
	config.want_print_full_vol_calib  =true;
	config.want_print_full_vol_calib_gnuplot  =false;
	config.want_print_exact_result    =true;
	config.want_print_full_vol_diag_compare=true;


	//// exact calib local
	//config.nbYear = 10;
	//config.calib_type = CalibrationType::GLOBAL;
	//config.re_init_randomly();//config.re_init();
	//test_LmmVirtual_gCalibration(config) ;

	//// exact calib local
	//config.nbYear = 10;
	//config.calib_type = CalibrationType::LOCAL;
	//config.re_init_randomly();//config.re_init();
	//test_LmmVirtual_gCalibration(config) ;

	////// exact cascade calib 
	//config.nbYear = 10;
	//config.calib_type = CalibrationType::CASCADE;
	//config.re_init_randomly();
	//test_LmmVirtual_gCalibration(config);

	config.nbYear = 10;
	config.penlt_libor = 0.5 ; config.penlt_time_homo = 1.;
	config.calib_type = CalibrationType::GLOBAL;
	config.re_init_randomly();//config.re_init();
	test_LmmVirtual_gCalibration(config) ;
}

void test_LmmVirtual_gCalibration(const Test_CalibrationConfig& config)
{
	config.print( config.get_basefilename() );

	if(config.want_print_full_vol_init){
		std::ostringstream file_init_vol; file_init_vol<<config.get_basefilename()<< "_full_init_vol.csv";
		config.pNoShifted_HGVolatilityParam->print( file_init_vol.str() );
	}
	const QuantLib::Array g_Delegate_Init = config.pGMatrixMapping->get_DelegateArray();
	const LowerTriangularDoubleMatrix g_full_init_matrix =  config.pGMatrixMapping->get_g_Ref();
	const UpperTriangularDoubleMatrix gDelegateTrueParam =  config.pGMatrixMapping->get_gDelegate_Ref();

	const QuantLib::Array g_Delegate_Perturbed = config.get_Perturbed_Array(g_Delegate_Init);	
	config.pGMatrixMapping->reset_gDelegate(g_Delegate_Perturbed);
	config.update_gDelegateChange();
	const LowerTriangularDoubleMatrix g_full_perturbed_matrix =  config.pGMatrixMapping->get_g_Ref();
	const GMatrixMapping copy_perturbed_GMatrixMapping(*config.pGMatrixMapping.get() );

	if(config.want_print_full_vol_perturb){
		std::ostringstream file_perturbed_vol; file_perturbed_vol<<config.get_basefilename()<< "_full_perturbed_vol.csv";
		config.pNoShifted_HGVolatilityParam->print( file_perturbed_vol.str() );
	}
	
	LmmBaseCalibrator_PTR pLmmBaseCalibrator;

	if(config.calib_type.isGlobal() )
	{
		pLmmBaseCalibrator.reset
			(
			new LmmGlobal_gCalibrator
			(
			copy_perturbed_GMatrixMapping
			, config.maxIter
			, config.x_epsilon
			, config.f_epsilon    
			, config.pLmmBaseCostFunction
			)
			);
		boost::shared_ptr<LmmGlobal_gCalibrator> pLmmGlobal_gCalibrator = boost::static_pointer_cast<LmmGlobal_gCalibrator>(pLmmBaseCalibrator);
		pLmmGlobal_gCalibrator->set_VirtualTestTrueParam(gDelegateTrueParam);
	}
	else if(config.calib_type.isLocal() )
	{
		pLmmBaseCalibrator.reset
			(
			new LmmLocal_gCalibrator
			(
			copy_perturbed_GMatrixMapping
			, config.maxIter
			, config.x_epsilon
			, config.f_epsilon    
			, config.pLmmBaseCostFunction
			)
			);
		boost::shared_ptr<LmmLocal_gCalibrator> pLmmLocal_gCalibrator = boost::static_pointer_cast<LmmLocal_gCalibrator>(pLmmBaseCalibrator);
		pLmmLocal_gCalibrator->set_VirtualTestTrueParam(gDelegateTrueParam);
	}
	else if(config.calib_type.isCascade() )
	{
		pLmmBaseCalibrator.reset
			(
			new LmmCascade_gCalibrator
			(
			copy_perturbed_GMatrixMapping
			, config.maxIter
			, config.x_epsilon
			, config.f_epsilon    
			, config.pLmmBaseCostFunction
			)
			);
		boost::shared_ptr<LmmCascade_gCalibrator> pLmmCascade_gCostFunction = boost::static_pointer_cast<LmmCascade_gCalibrator>(pLmmBaseCalibrator);
		pLmmCascade_gCostFunction->set_VirtualTestTrueParam(gDelegateTrueParam);
	}
		
	pLmmBaseCalibrator->solve();
		
	if(config.want_print_full_vol_calib){
		std::ostringstream file_calibrated_vol; file_calibrated_vol<<config.get_basefilename()<< "_full_calibrated_vol.csv";
		config.pNoShifted_HGVolatilityParam->print( file_calibrated_vol.str() );
	}

	const LowerTriangularDoubleMatrix g_full_calibrated_matrix =  config.pGMatrixMapping->get_g_Ref();
	if(config.want_print_exact_result){		
		std::ostringstream file_exact_calibration; file_exact_calibration<<config.get_basefilename()<< "_exact_calib.csv";
		pLmmBaseCalibrator->printPlusPlus( file_exact_calibration.str() ) ;
	}

	if(config.want_print_full_vol_diag_compare){
		std::ostringstream file_diag_compare; file_diag_compare<<config.get_basefilename()<< "_diag_compare.csv";
		Test_CalibrationConfig::printDiagComparison(file_diag_compare.str(),g_full_init_matrix,g_full_perturbed_matrix,g_full_calibrated_matrix );
	}


	if(config.want_print_full_vol_calib_gnuplot){
		LmmGnuplotPrinterMatrix gnuplot_printer(config.pLMMTenorStructure);
		std::ostringstream gnuplot_file; gnuplot_file<<config.get_basefilename()<< "gnuplot_gCalibrated";
		gnuplot_printer.printVolMatrix(g_full_calibrated_matrix,gnuplot_file.str());
	}
}

//void test_LmmVirtualCalibrationShift(const Test_CalibrationConfig& config)
//{
//	assert(config.nbYear>5);// nbYear>=6 in order to have nb of Skew Quote > nb Shift
//	config.pUpperTriangleSkewQuotes->print("test_LmmVirtualCalibrationShift_SkewQuotes.csv");
//
//	// reseting the use of shifted volatility
//	config.pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(config.pShifted_HGVolatilityParam);
//
//	//store a copy of init shifted param
//	config.pShifted_HGVolatilityParam->print("test_LmmVirtualCalibrationShift_initShift.csv");
//	const QuantLib::Array init_full_param = config.pShifted_HGVolatilityParam->get_ArrayFrom_Shift();
//
//	// perturb and reset the volatility param
//	QuantLib::Array copy_init_param(init_full_param);
//	std::vector<double> multiplicative_noise = Multiplicative::UniformNoise(copy_init_param.size(),config.perturbe_noise, config.unique_seed);
//	for(size_t i=0;i<copy_init_param.size();++i) copy_init_param[i]*= multiplicative_noise[i]; 
//	config.pShifted_HGVolatilityParam->reset_Shift_FromArray(copy_init_param);
//	config.pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(config.pShifted_HGVolatilityParam);
//
//	//store a copy of perturbed shifted param
//	config.pShifted_HGVolatilityParam->print("test_LmmVirtualCalibrationShift_perturbedShift.csv");
//	const QuantLib::Array perturb_full_param = config.pShifted_HGVolatilityParam->get_ArrayFrom_Shift();
//
//	LmmSkewCostFunction skew_cost_function(config.pLmmVanillaSwaptionApproxPricer_Rebonato, 
//		config.pLiborQuotes,
//		config.skew_strike_bump,
//		config.pUpperTriangleSkewQuotes,
//		config.pShifted_HGVolatilityParam);
//	skew_cost_function.reset_reference_calib(init_full_param);// for "virtual test" we know the "true" parameters
//
//	LmmShiftCalibrator shift_calibrator( perturb_full_param
//		, config.maxIter
//		, config.x_epsilon
//		, config.f_epsilon  
//		, skew_cost_function
//		);
//
//	shift_calibrator.solve();
//
//	//store a copy of calibrated shifted param
//	const QuantLib::Array calibrated_full_param = config.pShifted_HGVolatilityParam->get_ArrayFrom_Shift();
//	config.pShifted_HGVolatilityParam->print("test_LmmVirtualCalibrationShift_calibratedShift.csv");
//
//
//	std::string exact_calib_result = "test_LmmVirtualCalibrationShift_exact_result.csv";
//	Test_CalibrationConfig::printExacCalibResult(exact_calib_result,init_full_param,perturb_full_param,calibrated_full_param );
//	shift_calibrator.printPlusPlus( exact_calib_result ) ;
//
//
//	// reset the Rebonato approx to use the default no shifted volatility
//	config.pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(config.pNoShifted_HGVolatilityParam);
//}
