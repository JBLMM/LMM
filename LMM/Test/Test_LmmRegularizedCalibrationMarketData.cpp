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
#include <vector>

// ---- include for QuantLib calibration -------
#include <ql/termstructures/volatility/abcdcalibration.hpp>
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
#include <LMM/helper/InputFileManager.h>
#include <LMM/helper/LmmGnuplotPrinterMatrix.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>
#include <LMM/calibration/SwaptionMarketDataManager.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <LMM/LmmModel/LmmABCDCalibrator.h>
#include <LMM/LmmModel/LmmSkewCostFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/LmmSwaptionMarketData.h>

#include <LMM/LmmModel/LmmShiftCalibrator.h>

#include <LMM/LmmModel/LmmGlobal_gCalibrator.h>
#include <LMM/LmmModel/LmmGlobal_gCostFunction.h>
#include <LMM/LmmModel/LmmLocal_gCalibrator.h>
#include <LMM/LmmModel/LmmLocal_gCostFunction.h>
#include <LMM/LmmModel/LmmCascade_gCalibrator.h>
#include <LMM/LmmModel/LmmCascade_gCostFunction.h>

void print_matrices_error
	(
	const std::vector<double>& pel_time
	, const std::vector<double>& pel_libor
	, const boost::numeric::ublas::matrix<double>& quote_error_l2
	, const boost::numeric::ublas::matrix<double>& quote_error_l1
	, const boost::numeric::ublas::matrix<double>& quote_error_linf
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_l2
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_l1
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_linf
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_l2
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_l1
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_linf
	);

void generating_penalties_values(std::vector<double>& pel_time ,std::vector<double>& pel_libor );

void test_LmmRegularizedCalibrationMarketData_OneFile(const std::string& mkt_data_file);

void test_LmmRegularizedCalibrationMarketData()
{
	std::cout<<"hello test_LmmRegularizedCalibrationMarketData()"<<std::endl;

	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	test_LmmRegularizedCalibrationMarketData_OneFile(mkt_file_list[0]);
}


void test_LmmRegularizedCalibrationMarketData_OneFile(const std::string& mkt_data_file)
{

	LmmCalibrationConfig config;
	config.reset_nbYear(16);

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_file);

	config.correl_ReducedRank_= 30 ; config.correl_alpha_ = 0.000000001 ; config.correl_beta_  = 0.05;
	QuantLib::Array found_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

	config.correl_ReducedRank_= 3;
	config.use_positive_constraint_= true;

	std::vector<double> pel_time ; 
	std::vector<double> pel_libor; 

	generating_penalties_values(pel_time, pel_libor);

	const size_t penalty_matrix_size1 = pel_time.size();
	const size_t penalty_matrix_size2 = pel_libor.size();

	boost::numeric::ublas::matrix<double> quote_error_l2(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> quote_error_l1(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> quote_error_linf(penalty_matrix_size1,penalty_matrix_size2);

	boost::numeric::ublas::matrix<double> pel_timehomo_error_l2(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> pel_timehomo_error_l1(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> pel_timehomo_error_linf(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> pel_liborsmoth_error_l2(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> pel_liborsmoth_error_l1(penalty_matrix_size1,penalty_matrix_size2);
	boost::numeric::ublas::matrix<double> pel_liborsmoth_error_linf(penalty_matrix_size1,penalty_matrix_size2);


	for(size_t iPelTime=0;iPelTime<penalty_matrix_size1;++iPelTime)
	{
		for(size_t jPelLibor=0;jPelLibor<penalty_matrix_size2;++jPelLibor)
		{
			config.penalty_time_homogeneity_ = pel_time[iPelTime] ; 
			config.penalty_libor_            = pel_libor[jPelLibor];

			marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config), GMatrixMapping_PTR() );

			quote_error_l2(iPelTime,jPelLibor)          = config.result_quote_error_l2 ;
			quote_error_l1(iPelTime,jPelLibor)          = config.result_quote_error_l1 ;
			quote_error_linf(iPelTime,jPelLibor)        = config.result_quote_error_linf ;
			pel_timehomo_error_l2(iPelTime,jPelLibor)   = config.result_pelTime_error_l2 ;
			pel_timehomo_error_l1(iPelTime,jPelLibor)   = config.result_pelTime_error_l1 ;
			pel_timehomo_error_linf(iPelTime,jPelLibor) = config.result_pelTime_error_linf ;
			pel_liborsmoth_error_l2(iPelTime,jPelLibor) = config.result_pelLibor_error_l2 ;
			pel_liborsmoth_error_l1(iPelTime,jPelLibor) = config.result_pelLibor_error_l1 ;
			pel_liborsmoth_error_linf(iPelTime,jPelLibor) = config.result_pelLibor_error_linf ;

			config.reset_result();			
		}
	}

	print_matrices_error
		(
		pel_time
		,pel_libor
		,quote_error_l2
		,quote_error_l1
		,quote_error_linf
		,pel_timehomo_error_l2
		,pel_timehomo_error_l1
		,pel_timehomo_error_linf
		,pel_liborsmoth_error_l2
		,pel_liborsmoth_error_l1
		,pel_liborsmoth_error_linf
		);
}


void print_matrices_error
	(
	const std::vector<double>& pel_time
	, const std::vector<double>& pel_libor
	, const boost::numeric::ublas::matrix<double>& quote_error_l2
	, const boost::numeric::ublas::matrix<double>& quote_error_l1
	, const boost::numeric::ublas::matrix<double>& quote_error_linf
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_l2
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_l1
	, const boost::numeric::ublas::matrix<double>& pel_timehomo_error_linf
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_l2
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_l1
	, const boost::numeric::ublas::matrix<double>& pel_liborsmoth_error_linf
	)
{
	{ /// print to excel file

		size_t nbPelTime  = quote_error_l2.size1();
		size_t nbPelLibor = quote_error_l2.size2();

		std::string  all_error_file_name = "all_Regularization_Result.csv";
		std::string full_common_result_file = LMMPATH::get_Root_OutputPath() + all_error_file_name ;
		std::ofstream final_result ;

		final_result.open( full_common_result_file.c_str() );
		final_result<<"PelTime\\PelLibor ,,";
		for(size_t j=0;j<nbPelLibor;++j){ final_result<< pel_libor[j]<<" ,,"; } final_result<<std::endl;

		for(size_t iT=0;iT<nbPelTime;++iT)
		{
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<quote_error_l2(iT,jL)<<" ,,"; } final_result<<" L2 Quote Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<quote_error_l1(iT,jL)<<" ,,"; } final_result<<" L1 Quote Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<quote_error_linf(iT,jL)<<" ,,"; } final_result<<" L_INF Quote Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_timehomo_error_l2(iT,jL)<<" ,,"; } final_result<<" L2 Pel Time Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_timehomo_error_l1(iT,jL)<<" ,,"; } final_result<<" L1 Pel Time Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_timehomo_error_linf(iT,jL)<<" ,,"; } final_result<<" L_INF Pel Time Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_liborsmoth_error_l2(iT,jL)<<" ,,"; } final_result<<" L2 Pel Libor Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_liborsmoth_error_l1(iT,jL)<<" ,,"; } final_result<<" L1 Pel Libor Error ,"<<std::endl;
			final_result<<pel_time[iT]<<" ,," ; for(size_t jL=0;jL<nbPelLibor;++jL)	{ final_result<<pel_liborsmoth_error_linf(iT,jL)<<" ,,"; } final_result<<" L_INF Pel Libor Error ,"<<std::endl;

			final_result<<std::endl;
		}
		final_result<<std::endl<<std::endl;
		final_result.close();
	}

	{
		/// print in gnuplot format
		LmmGnuplotPrinterMatrix gnuplot_printer( create___LMMTenorStructure_PTR( 16 ) );
		gnuplot_printer.printFullMatrix(quote_error_l2 , pel_time,pel_libor , "gnuplot_quote_error_l2");
		gnuplot_printer.printFullMatrix(quote_error_l1 , pel_time,pel_libor , "gnuplot_quote_error_l1");
		gnuplot_printer.printFullMatrix(quote_error_linf , pel_time,pel_libor , "gnuplot_quote_error_linf");

		gnuplot_printer.printFullMatrix(pel_timehomo_error_l2 , pel_time,pel_libor , "gnuplot_pel_timehomo_error_l2");
		gnuplot_printer.printFullMatrix(pel_timehomo_error_l1 , pel_time,pel_libor , "gnuplot_pel_timehomo_error_l1");
		gnuplot_printer.printFullMatrix(pel_timehomo_error_linf , pel_time,pel_libor , "gnuplot_pel_timehomo_error_linf");

		gnuplot_printer.printFullMatrix(pel_liborsmoth_error_l2 , pel_time,pel_libor , "gnuplot_pel_liborsmoth_error_l2");
		gnuplot_printer.printFullMatrix(pel_liborsmoth_error_l1 , pel_time,pel_libor , "gnuplot_pel_liborsmoth_error_l1");
		gnuplot_printer.printFullMatrix(pel_liborsmoth_error_linf , pel_time,pel_libor , "gnuplot_pel_liborsmoth_error_linf");
	}
}

void generating_penalties_values(std::vector<double>& pel_time ,std::vector<double>& pel_libor )
{ 
	//// generating penalties values
	pel_time.clear();pel_libor.clear();

	pel_time.push_back(0.)     ; pel_libor.push_back(0.);  
	pel_time.push_back(0.0001) ; pel_libor.push_back(0.0001);  
	pel_time.push_back(0.001)  ; pel_libor.push_back(0.001);  
	pel_time.push_back(0.01)   ; pel_libor.push_back(0.01);  
	pel_time.push_back(0.1)    ; pel_libor.push_back(0.1);  
	pel_time.push_back(0.2)    ; pel_libor.push_back(0.2);  
	pel_time.push_back(0.4)    ; pel_libor.push_back(0.4);  
	pel_time.push_back(0.8)    ; pel_libor.push_back(0.8);  
	pel_time.push_back(1.)     ; pel_libor.push_back(1.);  

	std::sort(pel_time.begin()  , pel_time.end()  );
	std::sort(pel_libor.begin() , pel_libor.end() );
}