//#include <LMM/Test/Tests.h>
//
//#include <iostream>
//#include <cassert>
//#include <string.h>
//#include <cmath>
//#include <fstream>
//
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/uniform_real_distribution.hpp>
//#include <ctime>
//#include <cmath>
//
//// ---- include for QuantLib calibration -------
//#include <ql/math/optimization/endcriteria.hpp>
//#include <ql/math/optimization/constraint.hpp>
//#include <ql/math/optimization/problem.hpp>
//#include <ql/math/optimization/simplex.hpp>
//#include <ql/math/optimization/bfgs.hpp> 
//#include <ql/math/optimization/conjugategradient.hpp>
//#include <ql/math/optimization/levenbergmarquardt.hpp>
//// ---- include for QuantLib calibration -------
//
//#include <LMM/helper/GenericPath.h>
//#include <LMM/helper/TenorType.h>
//#include <LMM/helper/LMMTenorStructure.h>
//#include <LMM/numeric/NumericalMethods.h>
//#include <LMM/RNGenerator/McGenerator.h>
//#include <LMM/helper/Noise.h>
//
//#include <LMM/calibration/ATMSwaptionMarketData.h>
//#include <LMM/calibration/SwaptionMarketDataContainer.h>
//#include <LMM/calibration/SwaptionMarketDataManager.h>
//
//#include <LMM/LmmModel/Lmm.h>
//#include <LMM/LmmModel/Correlation.h>
//#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
//#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
//
//#include <LMM/calibration/CalibrationVolCostFunction.h>
//#include <LMM/calibration/TestHGVolatilityParam.h>
//
//
//class TestCalibrationConfig
//{
//public:
//	TestCalibrationConfig()
//		: tenorfixedleg(Tenor::_1YR)
//		, tenorfloatleg(Tenor::_6M)
//		, nbYear( 10 )
//	{
//		console_stop = false; // set to true if one wish to pause calibration process during iterations
//
//		a=0.22;b=0.4;c=0.6;d=0.15;
//		//nb_regularization_loop=2; //set to 1 if don't wish the penalty
//
//		perturbe_noise = 0.5; // relative perturbation noise 
//		g_diag_noise   = 0.1; // set to zero if one don't wish the noises on g matrix's diagonales
//		penalty_coeff = 1. ;//1./(double)(nbYear*nbYear) ;//100.;//
//		x_epsilon = 1e-10;
//		f_epsilon = 1e-8;
//		maxIter = 10000;
//		maxIterPenalty = 10;
//
//		unique_seed = 0;//NoiseHelper::get_Seed();	// note here seed that works, seed that bug, in order to reproduce
//
//		re_init_filenames();
//
//		sparse_step = tenorfixedleg.ratioTo(tenorfloatleg);	
//	}	
//
//	//bool isGlobal_Calibration(){return true;}
//	//bool isLocal_Calibration(){return true;}
//
//	// lmm config
//	Tenor tenorfixedleg; Tenor tenorfloatleg;
//	double a,b,c,d;
//	size_t nbYear,sparse_step;
//	//size_t nb_regularization_loop;
//
//	// calibration test noises config
//	unsigned int unique_seed;
//	double perturbe_noise, g_diag_noise ; // relative noises
//	double penalty_coeff;
//	double x_epsilon, f_epsilon;
//	size_t maxIter, maxIterPenalty;
//
//	// calibration test config
//	std::string basefilename,basefilename_global_calibration,basefilename_local_calibration;
//	bool console_stop;
//
//	mutable bool is_local_test; // test can modify this
//
//	void re_seed_randomly(){unique_seed = NoiseHelper::get_Seed(); re_init_filenames();}
//	void compute_penalty_growth(size_t i, size_t iMax)
//	{
//		double dist = iMax-i; 
//		penalty_coeff = exp( -dist );
//	}
//	void re_init_filenames();
//	void write_to_stream(std::ostream& os)const;
//	void print(const std::string& filename) const ;
//};
//std::ostream& operator<<(std::ostream& os, const TestCalibrationConfig& calib_config){calib_config.write_to_stream(os); return os;}
//
//SwaptionMarketDataContainer::VectorOfMatrixIndices createSparseIndice(size_t nbYear, size_t sparse_step);
//
//void setDiagonallyVolParam(QuantLib::Array & fullparam, const TestCalibrationConfig& config); 
//
//void testcase_SparseGlobalVolatilityCalibrator(const TestCalibrationConfig & test_config);
//void testcase_SparseLocalVolatilityCalibrator(const TestCalibrationConfig & test_config);
//void printToFileComparisonDiagVol(const ConstShifted_HGVolatilityParam& volInit, const ConstShifted_HGVolatilityParam& volRegulized, const std::string& filename);
//std::string printInfoLevenbergMarquardtSolver(QuantLib::Integer);
//void printFinalResultToFile(const std::string & basename, 
//							const ConstShifted_HGVolatilityParam_PTR initVol,  
//							const ConstShifted_HGVolatilityParam_PTR perturbVol,  
//							const ConstShifted_HGVolatilityParam_PTR calibrated,
//							const SwaptionMarketDataContainer_PTR mkt_data,
//							const LmmVanillaSwaptionApproxPricer_Rebonato_PTR approx,
//							const double & time_minimization,
//							const size_t total_NB_Iter
//							);
//
//LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(
//	const LMMTenorStructure_PTR pLMMTenorStructure, 
//	const ConstShifted_HGVolatilityParam_PTR pConstShifted_HGVolatilityParam);
//
//
//void test_CalibrationWithATMSwaptionVol()
//{
//	TestCalibrationConfig test_config;
//
//	//testcase_SparseLocalVolatilityCalibrator(test_config);
//	//testcase_SparseGlobalVolatilityCalibrator(test_config);
//
//	for(size_t i=0;i<20;++i)
//	{
//		test_config.re_seed_randomly();
//		testcase_SparseGlobalVolatilityCalibrator(test_config);
//	}
//}
//
//
//void testcase_SparseLocalVolatilityCalibrator(const TestCalibrationConfig & config)
//{
//	config.is_local_test = true;
//	std::cout<<config<<std::endl;
//
//
//	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(config.tenorfloatleg, config.nbYear) );
//	size_t nbLIBOR = pLmmTenorStructure->get_nbLIBOR();
//
//	size_t nbSparseParam = (config.nbYear-1)*config.nbYear/2 ;// = nbSwaption
//
//	SwaptionMarketDataContainer::VectorOfMatrixIndices all_sparse_indices = createSparseIndice(config.nbYear,config.sparse_step);
//
//
//	// =================   CREATION OF LMM_PTR   =============================
//
//	double a=config.a,b=config.b,c=config.c,d=config.d;
//	double g_constParam = 1.;
//	double shift_constParam = 0.0;
//
//	ConstShifted_HGVolatilityParam_PTR pCalibratedVolParam(new TestHGVolatilityParam(pLmmTenorStructure, a,b,c,d,g_constParam, shift_constParam));
//
//	const unsigned int nbFullParam = pCalibratedVolParam->get_nbCalibrateParam();
//
//	QuantLib::Array init_sparse_param(all_sparse_indices.size() , 1. , 0.); 
//	size_t index_step = config.tenorfixedleg.ratioTo(config.tenorfloatleg);
//	setDiagonallyVolParam(init_sparse_param, config);
//	//std::cout<<"Vol before Initiating; "<<std::endl <<*pCalibratedVolParam<<std::endl;
//	pCalibratedVolParam->reset_G_FromArray(init_sparse_param, all_sparse_indices , true); // reset with interpolated
//	//std::cout<<"Vol after Initiating; " <<std::endl<<*pCalibratedVolParam<<std::endl;
//
//	ConstShifted_HGVolatilityParam_PTR pCopyInitVolParam( new TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam)); // this Vol Param never change, aim to save a copy of initiated Vol
//	pCopyInitVolParam->reset_G_FromArray( pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() ), CalibrationVolCostFunction::VectorOfMatrixIndices() , false);
//	const QuantLib::Array init_full_param = pCopyInitVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	std::ostringstream file_init_vol; file_init_vol<<config.basefilename_local_calibration<< "_full_init_vol.csv";
//	pCopyInitVolParam->print(file_init_vol.str());
//
//
//	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer = createLmmVanillaSwaptionApproxPricer(pLmmTenorStructure,pCalibratedVolParam);
//
//	// =================  END  CREATION OF LMM_PTR   =============================
//
//	SwaptionMarketDataManager swaption_market_manager( pLmmTenorStructure ) ;
//	SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer = swaption_market_manager.generateMarketData(config.tenorfixedleg, config.tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),0.02,0);
//
//	size_t nbSwaption = pSwaptionMarketDataContainer->get_nbSWAPTION();
//	assert(nbSparseParam==nbSwaption);
//	const SwaptionMarketDataContainer::VectorOfMatrixIndices& indices_sparse_vol = pSwaptionMarketDataContainer->get_HGVOL_VECTOR_INDICES() ;
//	//preparing for deprecated constructor of 	CalibrationVolCostFunction
//	std::vector<double> libor_shifts(pSwaptionMarketDataContainer->get_nbLIBOR() );
//
//
//	CalibrationVolCostFunction::vector_ swaptionweights(nbSwaption,1.);        
//	CalibrationVolCostFunction::matrix_ weights_maturity(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_tenor(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_maturity_2(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_tenor_2(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//
//	//compute weight following swaption lenght
//	const SwaptionMarketDataContainer::SwaptionVector &  marketSwaptions  = pSwaptionMarketDataContainer->get_SWAPTION_VECTOR();
//
//	CalibrationVolCostFunction myFunc
//		(
//		pCalibratedVolParam, 
//		pSwaptionMarketDataContainer,
//		pLmmApproxVanillaSwaptionPricer,			
//		//libor_shifts,
//		swaptionweights,
//		weights_maturity,
//		weights_tenor,
//		weights_maturity_2,
//		weights_tenor_2	);
//	myFunc.reset_HG_Indices( indices_sparse_vol );
//	if(config.penalty_coeff>0) myFunc.use_penalty = true;
//
//
//	// ---- settting for calibration -------
//
//	std::vector<double> multiplicative_noise = Multiplicative::UniformNoise(nbSparseParam,config.perturbe_noise,config.unique_seed);
//	//std::vector<double> multiplicative_noise = Multiplicative::BernouilliNoise(nbSparseParam,perturbe_noise);
//
//	QuantLib::Array perturbed_sparse_param(init_sparse_param);
//	for(size_t i=0;i<nbSparseParam;++i){ perturbed_sparse_param[i]*= multiplicative_noise[i]; }
//
//	pCalibratedVolParam->reset_G_FromArray(perturbed_sparse_param, indices_sparse_vol , true); // reset with interpolated;
//	if(config.console_stop)
//		std::cout<<"Vol after perturbed ; " <<std::endl<<*pCalibratedVolParam<<std::endl;
//
//	ConstShifted_HGVolatilityParam_PTR pCopyPerturbedVolParam (new TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam)); // this Vol Param never change, aim to save a copy of perturbed Vol
//	pCopyPerturbedVolParam->reset_G_FromArray( pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() ), CalibrationVolCostFunction::VectorOfMatrixIndices() , false);
//	const QuantLib::Array perturbed_full_param = pCopyPerturbedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	std::ostringstream file_perturbed_vol; file_perturbed_vol<<config.basefilename_local_calibration<<"_full_perturbed_vol.csv";
//	pCopyPerturbedVolParam->print(file_perturbed_vol.str());
//
//
//	////////////////////////////////////////////////////////////////////////////////
//	// LOOP ITERATE AROUND SPARSE COLUMN
//	const std::vector<size_t>& maturities_indices = pSwaptionMarketDataContainer->get_SWPN_MATURITY_INDICES() ;
//	const SwaptionMarketDataContainer::MatrixOfMatrixIndices& indices_matrix = pSwaptionMarketDataContainer->get_HGVOL_NODE_MAPPING() ;
//	const SwaptionMarketDataContainer::SwaptionMatrix&  swaption_matrix = pSwaptionMarketDataContainer->get_SWPN_MATRIX();
//	const SwaptionMarketDataContainer::RealMatrix & vol_matrix =  pSwaptionMarketDataContainer->get_MKT_VOL_MATRIX() ;
//
//
//	size_t global_NB_Iter=0;
//	size_t nbColumn = indices_matrix.size();
//	assert(maturities_indices.size() == nbColumn);
//	clock_t start_calibration = std::clock();
//
//	for(size_t iCol=0;iCol<nbColumn;++iCol)
//	{
//		std::ostringstream filename;
//		//filename << config.basefilename_local_calibration<<"_calibratedVol_matur" <<maturities_indices[iCol]/2<<"YR.csv";
//		//std::string outfile_vol = filename.str(); 
//
//		const SwaptionMarketDataContainer::VectorOfMatrixIndices  & indices_column = indices_matrix[iCol];
//		const SwaptionMarketDataContainer::SwaptionVector         & swaption_line = swaption_matrix[iCol];
//		const SwaptionMarketDataContainer::RealVector             & swaption_vol_line = vol_matrix[iCol];
//
//		size_t nbSwaption_loc = indices_matrix[iCol].size();
//		CalibrationVolCostFunction::vector_ loc_swaptionweights(nbSwaption_loc,1.);   
//
//		// the higher column has less dependant variable, thus need a more severe stop criteria
//		QuantLib::Size maxIterations       = config.maxIter*(config.nbYear+iCol) ;
//		QuantLib::Size minStatIterations   = 50  ;
//		QuantLib::Real rootEpsilon         = config.x_epsilon; 
//		QuantLib::Real functionEpsilon     = config.f_epsilon;
//		QuantLib::Real gradientNormEpsilon = 0.   ;//1e-7 ;
//		QuantLib::EndCriteria myEndCrit ( maxIterations ,minStatIterations ,rootEpsilon ,functionEpsilon ,gradientNormEpsilon );
//		//QuantLib::NoConstraint constraint ;
//		QuantLib::PositiveConstraint constraint ;
//
//		myFunc.reset_HG_Indices( indices_column );
//		myFunc.reset_swpn_Vector( swaption_line );
//		myFunc.reset_mkt_vol_Vector( swaption_vol_line );
//		myFunc.reset_swaption_weights( loc_swaptionweights );
//
//		size_t lob_nbSparseParam = nbSwaption_loc;
//		const QuantLib::Array loc_init_sparse_param = pCopyInitVolParam->get_ArrayFrom_G( indices_column );
//		QuantLib::Array loc_perturbed_sparse_param = pCopyPerturbedVolParam->get_ArrayFrom_G( indices_column  );
//		assert(lob_nbSparseParam == loc_init_sparse_param.size() );
//		assert(lob_nbSparseParam == loc_perturbed_sparse_param.size() );
//
//		// Attention when setting use of LevenbergMarquardt Solver because its has its own tolerance parameters
//		// See
//		// ql\math\optimization\levenbergmarquardt.cpp
//		// ql\math\optimization\lmdif.cpp
//		// http://www.math.utah.edu/software/minpack/minpack/lmdif.html 
//		QuantLib::LevenbergMarquardt solver(functionEpsilon,rootEpsilon,1e-16) ;
//		//QuantLib::BFGS               solver ;		
//		//QuantLib::ConjugateGradient solver      ;
//		//QuantLib::Simplex           solver(0.1) ;
//
//		std::cout<<std::endl<<std::endl<< "-------  START CALIBRATION FOR SWAPTIONS MATURITY "<<maturities_indices[iCol]/2<<"YR" <<std::endl;
//
//		clock_t local_start_minimizer = std::clock();
//
//		std::cout<<"============================ testcase_MaturityCalibrator penalty_coeff = "<< config.penalty_coeff <<std::endl;
//
//		myFunc.reset_PenaltyCoeff(config.penalty_coeff);
//
//		loc_perturbed_sparse_param = pCopyPerturbedVolParam->get_ArrayFrom_G( indices_column  );
//
//		QuantLib::Problem myProb ( myFunc , constraint , loc_perturbed_sparse_param );
//
//		QuantLib::EndCriteria::Type solvedCrit = solver.minimize(myProb,myEndCrit);
//
//
//		clock_t local_end_minimizer = std::clock();
//		double local_time_minimization = double(local_start_minimizer - local_end_minimizer) / CLOCKS_PER_SEC;
//
//		QuantLib::Array loc_calibrated_sparse_param = pCalibratedVolParam->get_ArrayFrom_G( indices_column );
//
//		std::cout<<std::endl<< "SPWN Mat; local Minimisation time ; "<<std::endl
//			<< maturities_indices[iCol]<<"    ;"<<local_time_minimization <<";"<<std::endl;
//
//
//
//		std::cout<< " --  local Column comparison sparse init, perturbed, calibrated Vol Param --------;"<<std::endl
//			<<std::endl<< "InitParam ; PerturbedParam;	CalibratedParam;			;diffPerturb  ;  diffCalib"<<std::endl;
//		double l2_diff_calib=0,l2diff_perturb=0,linf_diff_calib=0,linf_diff_perturb=0;
//		for(size_t i=0;i<lob_nbSparseParam;++i)
//		{
//			std::cout<<loc_init_sparse_param[i]<<"              ;  "<<loc_perturbed_sparse_param[i]<<"           ;  "<<loc_calibrated_sparse_param[i]<<";			;";
//			double diffperturb = std::abs(loc_perturbed_sparse_param[i] - loc_init_sparse_param[i]);
//			double diffcalib = std::abs( loc_calibrated_sparse_param[i] - loc_init_sparse_param[i]);
//			std::cout<<diffperturb<<"  ;  " <<diffcalib<<";";
//			if(diffperturb > diffcalib)
//				std::cout<<"Ok;";
//			else
//				std::cout<<"Bad;";
//			std::cout<<std::endl;
//
//			l2_diff_calib+=diffcalib*diffcalib;
//			l2diff_perturb+=diffperturb*diffperturb;
//			if(linf_diff_calib<diffcalib) linf_diff_calib=diffcalib;
//			if(linf_diff_perturb<diffperturb) linf_diff_perturb=diffperturb;
//		}
//		std::cout<<std::endl;
//		std::cout<< "                                                     L2; "<<l2diff_perturb<<"  ;  "<<l2_diff_calib <<std::endl;
//		std::cout<< "                                                     L Inf; "<<linf_diff_perturb<<"  ;  "<<linf_diff_calib <<std::endl;
//
//		std::cout<< "---End  Calibration by Maturity "<<maturities_indices[iCol]/2<<"YR      nbCalled values:"<<myFunc.get_nbCalled()<<std::endl<<std::endl;
//		std::cout<< printInfoLevenbergMarquardtSolver(solver.getInfo()) <<std::endl;
//
//		if(config.console_stop){
//			std::cout<<"Test_RebonatoVolatilityCalibrator : type a key for continue ..."<<std::endl;getchar();
//		}
//
//		//pCalibratedVolParam->print(outfile_vol);
//		global_NB_Iter += myFunc.get_nbCalled() ; myFunc.reset_nbCalled();
//	}
//	clock_t end_calibration = std::clock();
//	double time_minimization = double(end_calibration - start_calibration) / CLOCKS_PER_SEC;
//
//	////////////////////////////////////////////////////////////////////////////////
//	// LOOP ITERATE AROUND SPARSE COLUMN
//
//	if(config.console_stop){
//		std::cout<<"Test_RebonatoVolatilityCalibrator : type a key for continue ..."<<std::endl;getchar();
//	}
//
//	std::ostringstream file_calibrated_vol; file_calibrated_vol<<config.basefilename_local_calibration<< "_full_calibrated_vol.csv";
//	pCalibratedVolParam->print(file_calibrated_vol.str());
//	const QuantLib::Array calibrated_full_param = pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//
//	printFinalResultToFile(config.basefilename_local_calibration,
//		pCopyInitVolParam,
//		pCopyPerturbedVolParam,
//		pCalibratedVolParam,
//		pSwaptionMarketDataContainer,
//		myFunc.get_LmmVanillaSwaptionApproxPricer(),
//		time_minimization,
//		global_NB_Iter);
//
//	{
//		TestHGVolatilityParam diff_init_perturbed_TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam); // this Vol Param never change, aim to save a copy of initiated Vol
//		TestHGVolatilityParam diff_init_calibrated_TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam); // this Vol Param never change, aim to save a copy of perturbed Vol
//
//		QuantLib::Array diff_init_perturbed  =  init_full_param - perturbed_full_param  ;
//		diff_init_perturbed_TestHGVolatilityParam.reset_G_FromArray(diff_init_perturbed , CalibrationVolCostFunction::VectorOfMatrixIndices());
//		std::ostringstream file_diff_perturbed; file_diff_perturbed<<config.basefilename_local_calibration<<"_diff_perturbed_vol.csv";
//		diff_init_perturbed_TestHGVolatilityParam.print(file_diff_perturbed.str() );
//
//		QuantLib::Array diff_init_calibrated = init_full_param - calibrated_full_param ;
//		diff_init_calibrated_TestHGVolatilityParam.reset_G_FromArray(diff_init_calibrated , CalibrationVolCostFunction::VectorOfMatrixIndices());
//		std::ostringstream file_diff_calibrated; file_diff_calibrated<<config.basefilename_local_calibration<<"_diff_calibrated_vol.csv";
//		diff_init_calibrated_TestHGVolatilityParam.print(file_diff_calibrated.str() );
//	}
//
//	config.print(config.basefilename_local_calibration);
//	if(config.penalty_coeff>0) //if use regularization, printout the diagonal comparison
//	{
//		std::string filediagcomparison = LMMPATH::get_output_path() + config.basefilename_local_calibration + "_diag_compare.csv";
//		printToFileComparisonDiagVol(*(pCopyInitVolParam.get()), *(pCalibratedVolParam.get()),filediagcomparison);		
//	}
//
//	///ctntodo
//	std::vector<double> simulatedImpliedVols;
//	/*! Compute simulation and implied volatilities
//	* Giving the calibrated volatility, and market's swaptions configurations (payments, maturities, date and tenor ) 
//	*	- use Monte-Carlo simulation to price the swaptions prices
//	*  - use NumericalMethod for returning these implied volatilities from theses prices
//	*  - compare with the market's volatilities
//	*/
//
//}
//
//
//void testcase_SparseGlobalVolatilityCalibrator(const TestCalibrationConfig & config)
//{
//	config.is_local_test = false;
//	std::cout<<config<<std::endl;
//
//	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(config.tenorfloatleg, config.nbYear) );
//	size_t nbLIBOR = pLmmTenorStructure->get_nbLIBOR();
//
//	size_t nbSparseParam = (config.nbYear-1)*config.nbYear/2 ;// = nbSwaption
//
//	SwaptionMarketDataContainer::VectorOfMatrixIndices all_sparse_indices = createSparseIndice(config.nbYear,config.sparse_step);
//
//
//	// =================   CREATION OF LMM_PTR   =============================
//
//	double a=config.a,b=config.b,c=config.c,d=config.d;
//	double g_constParam = 1.;
//	double shift_constParam = 0.0;
//
//	ConstShifted_HGVolatilityParam_PTR pCalibratedVolParam(new TestHGVolatilityParam(pLmmTenorStructure, a,b,c,d,g_constParam, shift_constParam));
//
//	const unsigned int nbFullParam = pCalibratedVolParam->get_nbCalibrateParam();
//
//	QuantLib::Array init_sparse_param(all_sparse_indices.size() , 1. , 0.); 
//	size_t index_step = config.tenorfixedleg.ratioTo(config.tenorfloatleg);
//	setDiagonallyVolParam(init_sparse_param, config);
//	//std::cout<<"Vol before Initiating; "<<std::endl <<*pCalibratedVolParam<<std::endl;
//	pCalibratedVolParam->reset_G_FromArray(init_sparse_param, all_sparse_indices , true); // reset with interpolated
//	//std::cout<<"Vol after Initiating; " <<std::endl<<*pCalibratedVolParam<<std::endl;
//
//	ConstShifted_HGVolatilityParam_PTR pCopyInitVolParam( new TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam)); // this Vol Param never change, aim to save a copy of initiated Vol
//	pCopyInitVolParam->reset_G_FromArray( pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() ), CalibrationVolCostFunction::VectorOfMatrixIndices() , false);
//	const QuantLib::Array init_full_param = pCopyInitVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	std::ostringstream file_init_vol; file_init_vol<<config.basefilename_global_calibration<< "_full_init_vol.csv";
//	pCopyInitVolParam->print(file_init_vol.str());
//
//	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer = createLmmVanillaSwaptionApproxPricer(pLmmTenorStructure,pCalibratedVolParam);
//
//	SwaptionMarketDataManager swaption_market_manager( pLmmTenorStructure ) ;
//	SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer = swaption_market_manager.generateMarketData(config.tenorfixedleg, config.tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),0.02,0.);
//
//	size_t nbSwaption = pSwaptionMarketDataContainer->get_nbSWAPTION();
//	assert(nbSparseParam==nbSwaption);
//	const SwaptionMarketDataContainer::VectorOfMatrixIndices& indices_sparse_vol = pSwaptionMarketDataContainer->get_HGVOL_VECTOR_INDICES() ;
//	//preparing for deprecated constructor of 	CalibrationVolCostFunction
//	std::vector<double> libor_shifts(pSwaptionMarketDataContainer->get_nbLIBOR() );
//
//
//	CalibrationVolCostFunction::vector_ swaptionweights(nbSwaption,1.);        
//	CalibrationVolCostFunction::matrix_ weights_maturity(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_tenor(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_maturity_2(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//	CalibrationVolCostFunction::matrix_ weights_tenor_2(nbLIBOR-1 , std::vector<double>(nbLIBOR-1,1.) );
//
//	//compute weight following swaption lenght
//	const SwaptionMarketDataContainer::SwaptionVector &  marketSwaptions  = pSwaptionMarketDataContainer->get_SWAPTION_VECTOR();
//
//	CalibrationVolCostFunction myFunc
//		(
//		pCalibratedVolParam, 
//		pSwaptionMarketDataContainer,
//		pLmmApproxVanillaSwaptionPricer,			
//		//libor_shifts,
//		swaptionweights,
//		weights_maturity,
//		weights_tenor,
//		weights_maturity_2,
//		weights_tenor_2	);
//	myFunc.reset_HG_Indices( indices_sparse_vol );
//	myFunc.use_penalty = false;
//
//	const QuantLib::Array true_param = pCopyInitVolParam->get_ArrayFrom_G( indices_sparse_vol );
//	myFunc.reset_reference_calib(true_param);
//
//	std::vector<double> multiplicative_noise = Multiplicative::UniformNoise(nbSparseParam,config.perturbe_noise,config.unique_seed);
//	//std::vector<double> multiplicative_noise = Multiplicative::BernouilliNoise(nbSparseParam,perturbe_noise);
//
//	QuantLib::Array perturbed_sparse_param(init_sparse_param);
//	for(size_t i=0;i<nbSparseParam;++i){ perturbed_sparse_param[i]*= multiplicative_noise[i]; }
//	pCalibratedVolParam->reset_G_FromArray(perturbed_sparse_param, indices_sparse_vol , true); // reset with interpolated;
//	if(config.console_stop)
//	{
//		std::cout<<"Vol after perturbed ; " <<std::endl<<*pCalibratedVolParam<<std::endl;
//	}
//
//	ConstShifted_HGVolatilityParam_PTR pCopyPerturbedVolParam (new TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam)); // this Vol Param never change, aim to save a copy of perturbed Vol
//	pCopyPerturbedVolParam->reset_G_FromArray( pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() ), CalibrationVolCostFunction::VectorOfMatrixIndices() , false);
//	const QuantLib::Array perturbed_full_param = pCopyPerturbedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	std::ostringstream file_perturbed_vol; file_perturbed_vol<<config.basefilename_local_calibration<<"_full_perturbed_vol.csv";
//	pCopyPerturbedVolParam->print(file_perturbed_vol.str());
//
//	//QuantLib::NoConstraint constraint ;
//	QuantLib::PositiveConstraint constraint ;
//
//	//QuantLib::BFGS              solver      ;
//	//QuantLib::ConjugateGradient solver      ;
//	//QuantLib::Simplex           solver(0.1) ;
//
//	//double penalty_step = 10.;
//	clock_t start_minimizer = std::clock();
//	size_t global_NB_Iter=0;
//
//	QuantLib::Array init_calib_sparse_param = pCopyPerturbedVolParam->get_ArrayFrom_G( indices_sparse_vol );
//	QuantLib::Problem myProb ( myFunc , constraint , init_calib_sparse_param );
//
//	QuantLib::Size maxIterations       = config.maxIter ;
//	QuantLib::Size minStatIterations   = 30  ;
//	QuantLib::Real rootEpsilon         = config.x_epsilon; 
//	QuantLib::Real functionEpsilon     = config.f_epsilon; 
//	QuantLib::Real gradientNormEpsilon = 0.   ;//1e-7 ;
//	QuantLib::EndCriteria myEndCrit ( maxIterations ,minStatIterations ,rootEpsilon ,functionEpsilon ,gradientNormEpsilon );
//
//	QuantLib::LevenbergMarquardt solver(functionEpsilon,rootEpsilon,1e-16) ;
//	QuantLib::EndCriteria::Type solvedCrit = solver.minimize(myProb,myEndCrit);
//
//	global_NB_Iter+= myFunc.get_nbCalled(); 
//
//
//	clock_t end_minimizer = std::clock();
//	double time_minimization = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
//
//	////   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP ///
//	if(config.penalty_coeff>0)
//	{// penalty loop
//		myFunc.use_penalty = true;
//		myFunc.reset_PenaltyCoeff(config.penalty_coeff);;
//		std::cout<<"============================ testcase_SparseGlobalVolatilityCalibrator penalty_coeff = "<< config.penalty_coeff <<std::endl;
//		myFunc.reset_nbCalled();
//
//		CalibrationVolCostFunction::vector_ p_swaptionweights(nbSwaption,0.);  
//		myFunc.reset_swaption_weights(p_swaptionweights);
//
//		QuantLib::PositiveConstraint p_constraint ;
//
//		clock_t p_start_minimizer = std::clock();
//		size_t p_global_NB_Iter=0;
//
//		QuantLib::Array p_init_calib_sparse_param = pCopyPerturbedVolParam->get_ArrayFrom_G( indices_sparse_vol );
//		QuantLib::Problem p_myProb ( myFunc , p_constraint , p_init_calib_sparse_param );
//
//		QuantLib::Size p_maxIterations       = config.maxIterPenalty ;
//		QuantLib::Size p_minStatIterations   = 5  ;
//		QuantLib::Real p_rootEpsilon         = 0.001 ;//config.x_epsilon; 
//		QuantLib::Real p_functionEpsilon     = 0.001 ;//config.f_epsilon; 
//		QuantLib::Real p_gradientNormEpsilon = 0.   ;//1e-7 ;
//		QuantLib::EndCriteria p_myEndCrit ( p_maxIterations ,p_minStatIterations ,p_rootEpsilon ,p_functionEpsilon ,p_gradientNormEpsilon );
//
//		QuantLib::LevenbergMarquardt p_solver(p_functionEpsilon,p_rootEpsilon,1e-16) ;
//		QuantLib::EndCriteria::Type p_solvedCrit = p_solver.minimize(myProb,myEndCrit);
//
//		p_global_NB_Iter+= myFunc.get_nbCalled(); 
//
//
//		clock_t p_end_minimizer = std::clock();
//		double p_time_minimization = double(p_end_minimizer - p_start_minimizer) / CLOCKS_PER_SEC;
//
//		time_minimization+=p_time_minimization;
//		global_NB_Iter+=p_global_NB_Iter;
//
//	}
//	////   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP   PENALTY LOOP ///
//
//
//	const QuantLib::Array calibrated_full_param = pCalibratedVolParam->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//
//	// ---- settting for calibration -------
//
//	if(config.console_stop){
//		std::cout<<"Test_RebonatoVolatilityCalibrator : type a key for continue ..."<<std::endl;getchar();
//	}
//
//	std::ostringstream file_calibrated_vol; file_calibrated_vol<<config.basefilename_global_calibration<< "_full_calibrated_vol.csv";
//	pCalibratedVolParam->print(file_calibrated_vol.str());
//
//
//	printFinalResultToFile(config.basefilename_global_calibration,
//		pCopyInitVolParam,
//		pCopyPerturbedVolParam,
//		pCalibratedVolParam,
//		pSwaptionMarketDataContainer,
//		myFunc.get_LmmVanillaSwaptionApproxPricer(),
//		time_minimization,
//		global_NB_Iter);
//
//	{
//		TestHGVolatilityParam diff_init_perturbed_TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam); // this Vol Param never change, aim to save a copy of initiated Vol
//		TestHGVolatilityParam diff_init_calibrated_TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam); // this Vol Param never change, aim to save a copy of perturbed Vol
//
//		QuantLib::Array diff_init_perturbed  =  init_full_param - perturbed_full_param  ;
//		diff_init_perturbed_TestHGVolatilityParam.reset_G_FromArray(diff_init_perturbed , CalibrationVolCostFunction::VectorOfMatrixIndices());
//		std::ostringstream file_diff_perturbed; file_diff_perturbed<<config.basefilename_global_calibration<<"_diff_perturbed_vol.csv";
//		diff_init_perturbed_TestHGVolatilityParam.print(file_diff_perturbed.str() );
//
//		QuantLib::Array diff_init_calibrated = init_full_param - calibrated_full_param ;
//		diff_init_calibrated_TestHGVolatilityParam.reset_G_FromArray(diff_init_calibrated , CalibrationVolCostFunction::VectorOfMatrixIndices());
//		std::ostringstream file_diff_calibrated; file_diff_calibrated<<config.basefilename_global_calibration<<"_diff_calibrated_vol.csv";
//		diff_init_calibrated_TestHGVolatilityParam.print(file_diff_calibrated.str() );
//	}
//
//	config.print(config.basefilename_global_calibration);
//
//	if(config.penalty_coeff>0) //if use regularization, printout the diagonal comparison
//	{
//		std::string filediagcomparison = LMMPATH::get_output_path() + config.basefilename_global_calibration + "_diag_compare.csv";
//		printToFileComparisonDiagVol(*(pCopyInitVolParam.get()), *(pCalibratedVolParam.get()),filediagcomparison);		
//	}
//
//	///ctntodo
//	std::vector<double> simulatedImpliedVols;
//	/*! Compute simulation and implied volatilities
//	* Giving the calibrated volatility, and market's swaptions configurations (payments, maturities, date and tenor ) 
//	*	- use Monte-Carlo simulation to price the swaptions prices
//	*  - use NumericalMethod for returning these implied volatilities from theses prices
//	*  - compare with the market's volatilities
//	*/
//
//}
//
//
//void setDiagonallyVolParam(QuantLib::Array & x, const TestCalibrationConfig& config)
//{
//
//	std::vector<std::vector<double> > fullvolmatrix;
//
//	size_t x_size = x.size();
//	size_t nb_alloc = 1;
//	size_t current_index = 0;
//
//	while (current_index < x_size)
//	{
//		std::vector<double> matrix_row;
//		for (size_t i = 0; i < nb_alloc; ++i)
//		{
//			matrix_row.push_back(x[current_index]);
//			current_index++;
//		}
//		fullvolmatrix.push_back( matrix_row );
//		nb_alloc++;
//	}
//
//	for (size_t i = 0; i < fullvolmatrix.size(); ++i)
//	{
//		size_t current_index = fullvolmatrix[i].size();
//		for (size_t j = current_index; j < fullvolmatrix.size(); ++j)
//			fullvolmatrix[i].push_back(0.);
//	}
//
//	size_t volmatrixDim = fullvolmatrix.size();
//
//	std::vector<unsigned int> diagonal_noise_seed(volmatrixDim);
//	for(size_t subdiagonalitem=0 ; subdiagonalitem<volmatrixDim;++subdiagonalitem)
//	{
//		//diag_noise
//		size_t nbElementOnSubSubDiagonal = volmatrixDim - subdiagonalitem ;		
//		diagonal_noise_seed[subdiagonalitem]= config.unique_seed +subdiagonalitem*1000 ;// each diag will sample with 'verry different' seed
//		std::vector<double> diag_mult_noise = Multiplicative::UniformNoise(nbElementOnSubSubDiagonal,config.g_diag_noise,diagonal_noise_seed[subdiagonalitem]);
//		//std::vector<double> diag_mult_noise = Multiplicative::BernouilliNoise(nbElementOnSubSubDiagonal,diag_noise);
//
//		for(size_t iCol=subdiagonalitem; iCol < volmatrixDim ; ++iCol)
//		{
//			size_t jCol = iCol - subdiagonalitem;
//
//			fullvolmatrix[iCol][jCol] = fullvolmatrix[iCol][jCol]* diag_mult_noise[jCol] ;
//			//fullvolmatrix[iCol][jCol] = (maxvol - increment*subdiagonalitem)* diag_mult_noise[jCol] ;			
//		}
//	}
//
//	size_t index_vector = 0;
//	for(size_t i=0;i<volmatrixDim ; ++i)
//	{
//		for(size_t j=0;j<=i ; ++j)
//		{
//			x[index_vector] = fullvolmatrix[i][j];	
//			++index_vector;
//		}
//	}
//	assert(index_vector == x.size() );
//
//
//	//std::ostringstream file_noise_init_diagonal;file_noise_init_diagonal << LMMPATH::get_output_path() <<  config.basefilename << "_NoisesDiagonal.csv";
//	//std::ofstream noise_stream;
//	//noise_stream.open(file_noise_init_diagonal.str());
//
//	//noise_stream<<"Storing Noise Seeder Information for Generating noised diagonal ,"<<std::endl
//	//	<<"Nb Diagonal, "<<volmatrixDim << ","<<std::endl
//	//	<<"Diag, Seeder"<<std::endl;
//	//for(size_t i=0;i<volmatrixDim;++i)
//	//{
//	//	noise_stream<<i<<","<<diagonal_noise_seed[i]<<","<<std::endl;
//	//}
//	//noise_stream.close();
//}
//
//
//SwaptionMarketDataContainer::VectorOfMatrixIndices createSparseIndice(size_t nbYear, size_t sparse_step)
//{
//	SwaptionMarketDataContainer::VectorOfMatrixIndices indices;
//	size_t max_index = sparse_step*nbYear;
//
//	for(size_t iYR=1;iYR<nbYear;++iYR)
//	{
//		for(size_t jYR=1;jYR<=iYR;++jYR)
//		{
//			size_t libor_index = iYR*sparse_step;
//			size_t time_index = jYR*sparse_step;
//			indices.push_back( std::pair<size_t,size_t>(libor_index,time_index) );
//		}	
//	}
//
//	return indices;
//}
//
//
//
////TestCalibrationConfig
//void TestCalibrationConfig::re_init_filenames()
//{
//
//	// setting base name files corresponding to specific test
//	std::ostringstream base_file_name;
//	base_file_name <<"test_"<<"reg_"<<penalty_coeff<<"_s"<<unique_seed<<"_SparseGlobalVolatilityCalibrator_"<<nbYear<<"YR";
//	basefilename_global_calibration = base_file_name.str(); base_file_name.str( std::string() );base_file_name.clear();
//	base_file_name <<"test_"<<"reg_"<<penalty_coeff<<"_s"<<unique_seed<<"_SparseLocalVolatilityCalibrator_"<<nbYear<<"YR";
//	basefilename_local_calibration = base_file_name.str(); base_file_name.str( std::string() );base_file_name.clear();
//	base_file_name <<"test_"<<"reg_"<<penalty_coeff<<"_s"<<unique_seed<<"_Common_"<<nbYear<<"YR";
//	basefilename = base_file_name.str();
//}
//
//
//void TestCalibrationConfig::write_to_stream(std::ostream& os)const
//{
//	os<< "nbYear,"<<nbYear<<",,  unique_seed,"<<unique_seed<<",,  penalty_coeff,"<<penalty_coeff <<"," <<std::endl<<std::endl;
//	os<< "g_diag_noise,"<<g_diag_noise<<","<<std::endl<<"perturbe_noise,"<<perturbe_noise<<","<<std::endl<<std::endl;
//	os<< "x_epsilon,"<<x_epsilon<<","<<std::endl<< "f_epsilon,"<<f_epsilon<<","<<std::endl<< "maxIter,"<<maxIter<<","<< "maxIterPenalty,"<<maxIterPenalty<<","<<std::endl<<std::endl;
//	os<< "a,b,c,d,"<<std::endl<<a<<","<<b<<","<<c<<","<<d<<","<<std::endl<<std::endl;
//	os<<"Common,"<<basefilename<<std::endl;
//	os<<"Global,"<<basefilename_global_calibration; if(!is_local_test)os<<",,,, YES";  os<<","<<std::endl;
//	os<<"Local,"<<basefilename_local_calibration; if(is_local_test)os<<",,,, YES";  os<<","<<std::endl<<std::endl;
//	os<< "Fixed Tenor,"<<tenorfixedleg<<","<<std::endl
//		<<"Float Tenor,"<<tenorfloatleg<<","<<std::endl
//		<<"Sparse Step,"<<sparse_step<<","<<std::endl;
//
//}
//
//void TestCalibrationConfig::print(const std::string& filename) const 
//{
//	std::string path_OutPut = LMMPATH::get_output_path() + filename;path_OutPut+="_config.csv";
//	std::ofstream configOut;
//
//	configOut.open(path_OutPut.c_str());
//	configOut<< *this <<std::endl;
//	configOut.close();
//}
//
//std::string printInfoLevenbergMarquardtSolver(QuantLib::Integer info)
//{
//	//http://www.math.utah.edu/software/minpack/minpack/lmdif.html see at the end
//
//	std::string LevenbergMarquardtSolverInfo;
//
//	if(info == 0)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 0.Improper input parameters" ;
//	else if(info == 1)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 1.Both actual and predicted relative reductions in the sum of squares are at most FTOL" ;
//	else if(info == 2)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 2.Relative error between two consecutive iterates is at most XTOL" ;
//	else if(info == 3)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 3.Conditions for INFO = 1 and INFO = 2 both hold." ;
//	else if(info == 4)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 4.The cosine of the angle between FVEC and any column of the Jacobian is at most GTOL in absolute value" ;
//	else if(info == 5)
//		std::cout<<" LevenbergMarquardtSolver 5.Number of calls to FCN has reached or exceeded MAXFEV" ;
//	else if(info == 5)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 6.FTOL is too small. No further reduction in the sum of squares is possible" ;
//	else if(info == 7)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 7.XTOL is too small. No further improvement in the approximate solution X is possible." ;
//	else if(info == 8)
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 8.GTOL is too small. FVEC is orthogonal to the	columns of the Jacobian to machine precision" ;
//	else
//		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver NOT DETERMINE INFO" ;
//
//	return LevenbergMarquardtSolverInfo;
//}
//
//void printToFileComparisonDiagVol(const ConstShifted_HGVolatilityParam& volInit, const ConstShifted_HGVolatilityParam& volRegulized, const std::string& filename)
//{
//	std::ofstream file_stream;
//	file_stream.open(filename);
//
//	file_stream<<"g diagonal difference Init vs Calibrated Vol,"<<std::endl<<std::endl;
//	size_t horizon = volInit.get_horizon(); assert(horizon==volRegulized.get_horizon());
//	size_t nb_diag = horizon-2;
//
//	for(size_t idiag=0;idiag<nb_diag;++idiag)
//	{
//		size_t nb_element_diag = horizon - idiag;
//
//		std::stringstream diagInitVol     ; diagInitVol<<"Init Diag g"<<idiag<<",";
//		std::stringstream diagRegulizedVol; diagRegulizedVol<<"Regu Diag g"<<idiag<<",";
//		std::stringstream diff; diff<<"Diff g"<<idiag<<",";
//
//		for(size_t element_diag=1;element_diag<=nb_element_diag;++element_diag)
//		{
//			size_t jCol = element_diag ;
//			size_t iRow = element_diag + idiag;
//			double gInit = volInit.g(iRow,jCol);
//			double gRegu = volRegulized.g(iRow,jCol);
//			double diffG = gInit - gRegu;
//
//			diagInitVol<<gInit<<",";
//			diagRegulizedVol<<gRegu<<",";
//			diff<<diffG <<",";
//		}
//
//		file_stream<<diagInitVol.str()<<std::endl;
//		file_stream<<diagRegulizedVol.str()<<std::endl;
//		file_stream<<diff.str()<<std::endl<<std::endl;
//	}
//
//	file_stream.close();
//}
//
//
//void printFinalResultToFile(const std::string & basename, 
//							const ConstShifted_HGVolatilityParam_PTR initVol,  
//							const ConstShifted_HGVolatilityParam_PTR perturbVol,  
//							const ConstShifted_HGVolatilityParam_PTR calibrated,
//							const SwaptionMarketDataContainer_PTR mkt_data,
//							const LmmVanillaSwaptionApproxPricer_Rebonato_PTR approx,
//							const double & time_minimization,
//							const size_t global_NB_Iter
//							)
//{
//	const QuantLib::Array init_full_param = initVol->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	const QuantLib::Array perturbed_full_param = perturbVol->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//	const QuantLib::Array calibrated_full_param = calibrated->get_ArrayFrom_G( CalibrationVolCostFunction::VectorOfMatrixIndices() );
//
//	size_t nbFullParam = init_full_param.size();
//
//	std::ostringstream file_compar_mkt_approx;file_compar_mkt_approx << LMMPATH::get_output_path() << basename <<"_Results.csv";
//	std::ofstream result_stream;
//	result_stream.open(file_compar_mkt_approx.str());
//
//	result_stream<<std::endl<< " -------COMPARISON INIT VOLPARAM vs CALIBRATED VOLPARAMS--------,"<<std::endl
//		<<std::endl<< "InitParam , PerturbedParam,	CalibratedParam, ,diffPerturb,  diffCalib,"<<std::endl;
//	double l2_diff_calib=0,l2diff_perturb=0,linf_diff_calib=0,linf_diff_perturb=0;
//	for(size_t i=0;i<nbFullParam;++i)
//	{
//		result_stream<<init_full_param[i]<<"              ,  "<<perturbed_full_param[i]<<"           ,  "<<calibrated_full_param[i]<<",,";
//		double diffperturb = std::abs(perturbed_full_param[i] - init_full_param[i]);
//		double diffcalib = std::abs( calibrated_full_param[i]- init_full_param[i]);
//		result_stream<<diffperturb<<"," <<diffcalib<<",";
//		if(diffperturb > diffcalib)
//			result_stream<<"Ok,";
//		else
//			result_stream<<"Bad,";
//		result_stream<<std::endl;
//
//		l2_diff_calib+=diffcalib*diffcalib;
//		l2diff_perturb+=diffperturb*diffperturb;
//		if(linf_diff_calib<diffcalib) linf_diff_calib=diffcalib;
//		if(linf_diff_perturb<diffperturb) linf_diff_perturb=diffperturb;
//	}
//	result_stream<<std::endl;
//	result_stream<< ",,,L2, "<<l2diff_perturb<<","<<l2_diff_calib <<std::endl;
//	result_stream<< ",,,L Inf, "<<linf_diff_perturb<<","<<linf_diff_calib <<std::endl;
//
//	const SwaptionMarketDataContainer::RealVector &      marketInitLIBORs = mkt_data->get_LIBOR_INIT();
//	const SwaptionMarketDataContainer::RealVector &      marketSwapRates  = mkt_data->get_STRIKE_VECTOR();
//	const SwaptionMarketDataContainer::SwaptionVector &  marketSwaptions  = mkt_data->get_SWAPTION_VECTOR();
//	const SwaptionMarketDataContainer::RealVector &      marketVols       = mkt_data->get_VOLATILITY_VECTOR();
//	std::vector<VanillaSwap> marketSwaps;
//	for (const auto& swaption : marketSwaptions)
//	{
//		marketSwaps.push_back( swaption.getUnderlyingSwap() );
//	}
//	size_t nbSwaption = marketSwaptions.size();
//
//	std::vector<double> approxSwaptionVols;
//	for (const auto& swaption : marketSwaptions)
//	{
//		double approxVol = approx->volBlack(swaption,marketInitLIBORs);
//		approxSwaptionVols.push_back( approxVol );
//	}
//
//	result_stream<<std::endl<< " -------COMPARISON MARKET's VOL vs APPROXIMATION CALIBRATED VOL --------,"<<std::endl;
//	result_stream<< "Market ,"<<"	Approx,"<<std::endl;
//	double L2_diff=0.;
//	double Linf_diff=0.;
//	for(size_t i=0;i<nbSwaption;++i)
//	{
//		result_stream<< marketVols[i]<<",	"<< approxSwaptionVols[i]<<","<<std::endl;
//		L2_diff += std::abs(marketVols[i]*marketVols[i] - approxSwaptionVols[i]*approxSwaptionVols[i]);
//		if(Linf_diff < std::abs(marketVols[i] - approxSwaptionVols[i]) )
//			Linf_diff= std::abs(marketVols[i] - approxSwaptionVols[i]);
//	}
//	result_stream<< ",,L2 normalized diff Approx vs Market , "<<std::sqrt(L2_diff)/nbSwaption<<","<<std::endl;
//	result_stream<< ",,LInf normalized diff Approx vs Market , "<<Linf_diff<<","<<std::endl;
//	result_stream<< ",,Total time  , "<<time_minimization<<","<<std::endl;
//	result_stream<< ",,nbCalled values, "<<global_NB_Iter<<std::endl;
//
//	result_stream<< "########,########,########,########,########,"<<std::endl<<std::endl<<std::endl;
//
//	result_stream.close();
//}
//
//LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(
//	const LMMTenorStructure_PTR pLMMTenorStructure, 
//	const ConstShifted_HGVolatilityParam_PTR pConstShifted_HGVolatilityParam)
//{
//	//! Correlation 
//	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
//	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
//	size_t correlReducedRank = nbFactor;
//	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
//	double correlAlpha = 0.0;
//	double correlBeta  = 0.1;
//	//Correlation_PTR correlation(new Const_Correlation(correlFullRank,correlReducedRank, correlReductionType,0.)); //not work
//	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
//	correlation->calculate(); // for print.
//
//
//	double fwdRate = 0.04;
//	std::vector<double> liborsInitValue(pLMMTenorStructure->get_horizon()+1, fwdRate);
//
//	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, correlation, pConstShifted_HGVolatilityParam)); 
//
//	//! Dispersion
//	Dispersion dispersion(pVolatilityFunction);
//
//	Lmm_PTR lmm_ptr(new Lmm(dispersion) );
//
//	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	
//
//	return pLmmApproxVanillaSwaptionPricer;
//}