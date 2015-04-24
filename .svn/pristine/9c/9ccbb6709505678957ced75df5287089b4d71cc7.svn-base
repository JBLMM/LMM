#include <LMM/Test/Tests.h>

#include <ctime>

#include <LMM/helper/GenericPath.h>
#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>
#include <LMM/calibration/SwaptionMarketDataManager.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Dispersion.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/pricer/LmmVanillaSwapPricer.h>

LmmVanillaSwaptionApproxPricer_Rebonato_PTR create_LmmVanillaSwaptionApproxPricer(const Tenor& tenorfixedleg,const Tenor& tenorfloatleg,LMMTenorStructure_PTR pLmmTenorStructure, const double const_shift );
void test_generatingATMSwaptionMarketData(size_t nbYear, const double const_rate);
void test_generatingMoneynessSwaptionMarketData(size_t nbYear, const double const_rate, const double const_shift, const double strike_bump);

void test_SwaptionMarketDataContainer()
{
	size_t nbYear = 4;
	const double strike_bump = 0.001;
	const double constant_rate  = 0.02;
	const double constant_shift = 0.01;

	test_generatingATMSwaptionMarketData(nbYear,constant_rate);                                   // ATM

	test_generatingMoneynessSwaptionMarketData(nbYear,constant_rate,constant_shift, strike_bump); // ATM++ and ATM--
}


void test_generatingATMSwaptionMarketData(size_t nbYear, const double const_rate)
{
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );
	size_t nbLIBOR = pLmmTenorStructure->get_nbLIBOR();

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer = create_LmmVanillaSwaptionApproxPricer(tenorfixedleg,tenorfloatleg,pLmmTenorStructure,0. );

	SwaptionMarketDataManager swaption_market_manager( pLmmTenorStructure ) ;

	clock_t start_data_creation = std::clock();	
	SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer = swaption_market_manager.generateMarketData(tenorfixedleg, tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),const_rate,0.);
	clock_t end_data_creation = std::clock();

	double time_data_creation = double(end_data_creation - start_data_creation) / CLOCKS_PER_SEC;


	size_t nbLibor     = pSwaptionMarketDataContainer->get_nbLIBOR()  ;
	size_t nbSwaption  = pSwaptionMarketDataContainer->get_nbSWAPTION() ;
	size_t sparse_step = pSwaptionMarketDataContainer->get_SwaptionSparseStep() ;

	// created interest rate data 
	const std::vector<double> & libor_start_dates = pSwaptionMarketDataContainer->get_LIBOR_STARTDATES() ;
	const std::vector<double> & libor_init        = pSwaptionMarketDataContainer->get_LIBOR_INIT()       ;
	const std::vector<double> & zc_maturities     = pSwaptionMarketDataContainer->get_ZC_MATURITIES()    ;
	const std::vector<double> & zc                = pSwaptionMarketDataContainer->get_ZC_BOND()          ;	
	const std::vector<double> & numeraire         = pSwaptionMarketDataContainer->get_NUMERAIRE()        ;

	// created swaptions data 

	const SwaptionMarketDataContainer::SwaptionVector &  swaptionvector =     pSwaptionMarketDataContainer->get_SWAPTION_VECTOR() ;

	const std::vector<size_t>& maturities_indices = pSwaptionMarketDataContainer->get_SWPN_MATURITY_INDICES() ;

	const SwaptionMarketDataContainer::MatrixOfMatrixIndices& indices_matrix = pSwaptionMarketDataContainer->get_HGVOL_NODE_MAPPING() ;

	const SwaptionMarketDataContainer::VectorOfMatrixIndices& full_indices_vector = pSwaptionMarketDataContainer->get_HGVOL_VECTOR_INDICES() ;

	const SwaptionMarketDataContainer::SwaptionMatrix&  swaption_matrix = pSwaptionMarketDataContainer->get_SWPN_MATRIX();

	const SwaptionMarketDataContainer::RealVector & full_vol = pSwaptionMarketDataContainer->get_VOLATILITY_VECTOR()   ;

	const SwaptionMarketDataContainer::RealMatrix & vol_matrix =  pSwaptionMarketDataContainer->get_MKT_VOL_MATRIX() ;

	std::cout<<"test_generatingATMSwaptionMarketData()"<<std::endl
		<<"	Creation Data for " <<nbYear <<"YR,   Time consum for creation = "<<time_data_creation<<"s"<<std::endl;
	
	LmmVanillaSwapPricer_PTR swap_pricer_ptr ( new LmmVanillaSwapPricer(pLmmTenorStructure) );
	bool is_data_ATM_Swaption = pSwaptionMarketDataContainer->is_ATMSwaptionData(swap_pricer_ptr);
	bool is_data_consistant = pSwaptionMarketDataContainer->check_data_consistance();
	
	pSwaptionMarketDataContainer->print("test_generatingATMSwaptionMarketData.csv");
}


void test_generatingMoneynessSwaptionMarketData(size_t nbYear, const double const_rate, const double const_shift, const double strike_bump)
{
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );
	size_t nbLIBOR = pLmmTenorStructure->get_nbLIBOR();

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer = create_LmmVanillaSwaptionApproxPricer(tenorfixedleg,tenorfloatleg,pLmmTenorStructure,const_shift );

	SwaptionMarketDataManager swaption_market_manager( pLmmTenorStructure ) ;

	{
		SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer_pp = swaption_market_manager.generateMarketData(tenorfixedleg, tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),const_rate,strike_bump);

		size_t nbLibor     = pSwaptionMarketDataContainer_pp->get_nbLIBOR()  ;
		size_t nbSwaption  = pSwaptionMarketDataContainer_pp->get_nbSWAPTION() ;
		size_t sparse_step = pSwaptionMarketDataContainer_pp->get_SwaptionSparseStep() ;

		// created interest rate data 
		const std::vector<double> & libor_start_dates = pSwaptionMarketDataContainer_pp->get_LIBOR_STARTDATES() ;
		const std::vector<double> & libor_init        = pSwaptionMarketDataContainer_pp->get_LIBOR_INIT()       ;
		const std::vector<double> & zc_maturities     = pSwaptionMarketDataContainer_pp->get_ZC_MATURITIES()    ;
		const std::vector<double> & zc                = pSwaptionMarketDataContainer_pp->get_ZC_BOND()          ;	
		const std::vector<double> & numeraire         = pSwaptionMarketDataContainer_pp->get_NUMERAIRE()        ;

		// created swaptions data 

		const SwaptionMarketDataContainer::SwaptionVector &  swaptionvector =     pSwaptionMarketDataContainer_pp->get_SWAPTION_VECTOR() ;

		const std::vector<size_t>& maturities_indices = pSwaptionMarketDataContainer_pp->get_SWPN_MATURITY_INDICES() ;

		const SwaptionMarketDataContainer::MatrixOfMatrixIndices& indices_matrix = pSwaptionMarketDataContainer_pp->get_HGVOL_NODE_MAPPING() ;

		const SwaptionMarketDataContainer::VectorOfMatrixIndices& full_indices_vector = pSwaptionMarketDataContainer_pp->get_HGVOL_VECTOR_INDICES() ;

		const SwaptionMarketDataContainer::SwaptionMatrix&  swaption_matrix = pSwaptionMarketDataContainer_pp->get_SWPN_MATRIX();

		const SwaptionMarketDataContainer::RealVector & full_vol = pSwaptionMarketDataContainer_pp->get_VOLATILITY_VECTOR()   ;

		const SwaptionMarketDataContainer::RealMatrix & vol_matrix =  pSwaptionMarketDataContainer_pp->get_MKT_VOL_MATRIX() ;

		bool is_data_consistant = pSwaptionMarketDataContainer_pp->check_data_consistance();
		pSwaptionMarketDataContainer_pp->print("test_generatingMoneynessSwaptionMarketData_ATMpp.csv");
	}

	{
		SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer_mm = swaption_market_manager.generateMarketData(tenorfixedleg, tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),const_rate,-strike_bump);

		size_t nbLibor     = pSwaptionMarketDataContainer_mm->get_nbLIBOR()  ;
		size_t nbSwaption  = pSwaptionMarketDataContainer_mm->get_nbSWAPTION() ;
		size_t sparse_step = pSwaptionMarketDataContainer_mm->get_SwaptionSparseStep() ;

		// created interest rate data 
		const std::vector<double> & libor_start_dates = pSwaptionMarketDataContainer_mm->get_LIBOR_STARTDATES() ;
		const std::vector<double> & libor_init        = pSwaptionMarketDataContainer_mm->get_LIBOR_INIT()       ;
		const std::vector<double> & zc_maturities     = pSwaptionMarketDataContainer_mm->get_ZC_MATURITIES()    ;
		const std::vector<double> & zc                = pSwaptionMarketDataContainer_mm->get_ZC_BOND()          ;	
		const std::vector<double> & numeraire         = pSwaptionMarketDataContainer_mm->get_NUMERAIRE()        ;

		// created swaptions data 

		const SwaptionMarketDataContainer::SwaptionVector &  swaptionvector =     pSwaptionMarketDataContainer_mm->get_SWAPTION_VECTOR() ;

		const std::vector<size_t>& maturities_indices = pSwaptionMarketDataContainer_mm->get_SWPN_MATURITY_INDICES() ;

		const SwaptionMarketDataContainer::MatrixOfMatrixIndices& indices_matrix = pSwaptionMarketDataContainer_mm->get_HGVOL_NODE_MAPPING() ;

		const SwaptionMarketDataContainer::VectorOfMatrixIndices& full_indices_vector = pSwaptionMarketDataContainer_mm->get_HGVOL_VECTOR_INDICES() ;

		const SwaptionMarketDataContainer::SwaptionMatrix&  swaption_matrix = pSwaptionMarketDataContainer_mm->get_SWPN_MATRIX();

		const SwaptionMarketDataContainer::RealVector & full_vol = pSwaptionMarketDataContainer_mm->get_VOLATILITY_VECTOR()   ;

		const SwaptionMarketDataContainer::RealMatrix & vol_matrix =  pSwaptionMarketDataContainer_mm->get_MKT_VOL_MATRIX() ;

		bool is_data_consistant = pSwaptionMarketDataContainer_mm->check_data_consistance();
		pSwaptionMarketDataContainer_mm->print("test_generatingMoneynessSwaptionMarketData_ATMmm.csv");
	}

	{
		SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer_skew = swaption_market_manager.generateSkewData(tenorfixedleg, tenorfloatleg, pLmmApproxVanillaSwaptionPricer.get(),const_rate,strike_bump);

		size_t nbLibor     = pSwaptionMarketDataContainer_skew->get_nbLIBOR()  ;
		size_t nbSwaption  = pSwaptionMarketDataContainer_skew->get_nbSWAPTION() ;
		size_t sparse_step = pSwaptionMarketDataContainer_skew->get_SwaptionSparseStep() ;

		// created interest rate data 
		const std::vector<double> & libor_start_dates = pSwaptionMarketDataContainer_skew->get_LIBOR_STARTDATES() ;
		const std::vector<double> & libor_init        = pSwaptionMarketDataContainer_skew->get_LIBOR_INIT()       ;
		const std::vector<double> & zc_maturities     = pSwaptionMarketDataContainer_skew->get_ZC_MATURITIES()    ;
		const std::vector<double> & zc                = pSwaptionMarketDataContainer_skew->get_ZC_BOND()          ;	
		const std::vector<double> & numeraire         = pSwaptionMarketDataContainer_skew->get_NUMERAIRE()        ;

		// created swaptions data 

		const SwaptionMarketDataContainer::SwaptionVector &  swaptionvector =     pSwaptionMarketDataContainer_skew->get_SWAPTION_VECTOR() ;

		const std::vector<size_t>& maturities_indices = pSwaptionMarketDataContainer_skew->get_SWPN_MATURITY_INDICES() ;

		const SwaptionMarketDataContainer::MatrixOfMatrixIndices& indices_matrix = pSwaptionMarketDataContainer_skew->get_HGVOL_NODE_MAPPING() ;

		const SwaptionMarketDataContainer::VectorOfMatrixIndices& full_indices_vector = pSwaptionMarketDataContainer_skew->get_HGVOL_VECTOR_INDICES() ;

		const SwaptionMarketDataContainer::SwaptionMatrix&  swaption_matrix = pSwaptionMarketDataContainer_skew->get_SWPN_MATRIX();

		const SwaptionMarketDataContainer::RealVector & full_vol = pSwaptionMarketDataContainer_skew->get_VOLATILITY_VECTOR()   ;

		const SwaptionMarketDataContainer::RealMatrix & vol_matrix =  pSwaptionMarketDataContainer_skew->get_MKT_VOL_MATRIX() ;

		const double strike_bump = pSwaptionMarketDataContainer_skew->get_StrikeBump();

		LmmVanillaSwapPricer_PTR swap_pricer_ptr ( new LmmVanillaSwapPricer(pLmmTenorStructure) );
		bool is_data_ATM_Swaption = pSwaptionMarketDataContainer_skew->is_ATMSwaptionData(swap_pricer_ptr);

		bool is_data_consistant = pSwaptionMarketDataContainer_skew->check_data_consistance();
		pSwaptionMarketDataContainer_skew->print("test_generatingMoneynessSwaptionMarketData_skew.csv");
	}
}


LmmVanillaSwaptionApproxPricer_Rebonato_PTR create_LmmVanillaSwaptionApproxPricer(const Tenor& tenorfixedleg,const Tenor& tenorfloatleg,LMMTenorStructure_PTR pLmmTenorStructure, const double const_shift )
{
	// =================   CREATION OF LMM_PTR   =============================
	//init abcd default a=-0.06,  b=0.17, c=0.54, d=0.17
	double a=0.22,b=0.4,c=0.6,d=0.15;

	double g_constParam = 1.0;
	double shift_constParam = const_shift;
	ConstShifted_HGVolatilityParam_PTR volParams_ptr(new TestHGVolatilityParam(pLmmTenorStructure,a,b,c,d, g_constParam, shift_constParam) );
	//HGVolatilityParam_PTR volParams_ptr(new DefaultHGVolatilityParam(a,b,c,d,pLmmTenorStructure) );

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