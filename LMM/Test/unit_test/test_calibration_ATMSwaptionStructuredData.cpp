#define BOOST_TEST_MODULE LMM_unit_test_calibration_ATMSwaptionStructuredData
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/ATMSwaptionStructuredData.h>



BOOST_AUTO_TEST_SUITE(lmm_test_calibration_ATMSwaptionStructuredData)

BOOST_AUTO_TEST_CASE(test_ATMSwaptionStructuredDataTest)
{	
	Tenor tenorlmm = Tenor::_6M;
	Tenor tenorfixedleg = Tenor::_6M;
	Tenor tenorfloatleg = Tenor::_6M;
	size_t nbYear = 3;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorlmm, nbYear) );

	ATMSwaptionStructuredData atpSwaptionStructuredData( pLmmTenorStructure );

	//ctntodo create a very simple data file .csv in order to exactly check all of the data 
	// and parsing behavior
	std::string path_FileIn = LMM::get_runtime_datapath() + "testATMSwaption_Mtk_info.csv";

	ATMSwaptionMarketData atmSwaptionMarketData;
	atmSwaptionMarketData.readDataFromFile( path_FileIn );

	atpSwaptionStructuredData.parseFromMarketData(atmSwaptionMarketData , tenorfixedleg, tenorfloatleg);

	const std::vector<double> &  lmmTimeLine         = pLmmTenorStructure->get_tenorDate()              ;
	const std::vector<double> &  libor_startdates_BB = atpSwaptionStructuredData.get_LIBOR_STARTDATES() ;
	const std::vector<double> &  libors_BB           = atpSwaptionStructuredData.get_LIBOR_INIT()       ;
	const std::vector<double> &  libors_shift_BB     = atpSwaptionStructuredData.get_LIBOR_SHIFT()      ;	
	const std::vector<double> &  zc_Maturities_BB    = atpSwaptionStructuredData.get_ZC_MATURITIES()    ;
	const std::vector<double> &  zc_BB               = atpSwaptionStructuredData.get_ZC_BOND()          ;
	const std::vector<double> &  numeraires_BB       = atpSwaptionStructuredData.get_NUMERAIRE()        ;

	const ATMSwaptionStructuredData::RealVector &     swaprates_BB         = atpSwaptionStructuredData.get_SWAPRATE_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     swaptionVolVector_BB = atpSwaptionStructuredData.get_VOLATILITY_VECTOR() ;
	const ATMSwaptionStructuredData::SwaptionVector & swaptionVector_BB    = atpSwaptionStructuredData.get_SWAPTION_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     annuityVector_BB     = atpSwaptionStructuredData.get_ANNUITY_0_VECTOR()  ;	
	const ATMSwaptionStructuredData::RealMatrix&      omega0Matrix_BB      = atpSwaptionStructuredData.get_OMEGA_0_MATRIX()    ;

	BOOST_CHECK( atpSwaptionStructuredData.check_data_consistance() );

	atpSwaptionStructuredData.print("unitTest_ATMSwaptionStructuredData.csv");

	// setting for all swaprate and strikes to 100.
	size_t nbSwaption = atpSwaptionStructuredData.get_nbSWAPTION();
	for(size_t i=0;i<nbSwaption;++i)
	{
		atpSwaptionStructuredData.strike(i)   = 100. ;
		atpSwaptionStructuredData.swaprate(i) = 100. ;
	}

	const ATMSwaptionStructuredData::RealVector     & swaprate_vector = atpSwaptionStructuredData.get_SWAPRATE_VECTOR();
	const ATMSwaptionStructuredData::SwaptionVector & swaption_vector = atpSwaptionStructuredData.get_SWAPTION_VECTOR();
	
	// check if setting is effective
	for(size_t i=0;i<nbSwaption;++i)
	{
		BOOST_CHECK( swaprate_vector[i]              == 100. ) ;
		BOOST_CHECK( swaption_vector[i].get_strike() == 100. ) ;
	}

	// resetting for libor shifts to 1000.
	size_t nbLIBOR = atpSwaptionStructuredData.get_nbLIBOR();
	for(size_t kLibor = 0; kLibor < nbLIBOR ; ++kLibor)
	{
		atpSwaptionStructuredData.libor_shift( kLibor ) = 1000. ;
	}

	// check if setting libor shift is effective
	const std::vector<double> &  libors_shift_vector = atpSwaptionStructuredData.get_LIBOR_SHIFT() ;
	for(size_t kLibor=0;kLibor<nbLIBOR;++kLibor)
	{
		BOOST_CHECK( libors_shift_vector[kLibor] == 1000. );
	}
}

BOOST_AUTO_TEST_CASE(test_ATMSwaptionStructured1)
{	
	Tenor tenorlmm = Tenor::_3M;
	Tenor tenorfixedleg = Tenor::_6M;
	Tenor tenorfloatleg = Tenor::_3M;
	size_t nbYear = 2;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorlmm, nbYear) );

	ATMSwaptionStructuredData atpSwaptionStructuredData( pLmmTenorStructure );

	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_1.csv";

	ATMSwaptionMarketData atmSwaptionMarketData;
	atmSwaptionMarketData.readDataFromFile( path_FileIn );

	atpSwaptionStructuredData.parseFromMarketData(atmSwaptionMarketData , tenorfixedleg, tenorfloatleg);

	const std::vector<double> &  lmmTimeLine         = pLmmTenorStructure->get_tenorDate()              ;
	const std::vector<double> &  libor_startdates_BB = atpSwaptionStructuredData.get_LIBOR_STARTDATES() ;
	const std::vector<double> &  libors_BB           = atpSwaptionStructuredData.get_LIBOR_INIT()       ;
	const std::vector<double> &  libors_shift_BB     = atpSwaptionStructuredData.get_LIBOR_SHIFT()      ;	
	const std::vector<double> &  zc_Maturities_BB    = atpSwaptionStructuredData.get_ZC_MATURITIES()    ;
	const std::vector<double> &  zc_BB               = atpSwaptionStructuredData.get_ZC_BOND()          ;	
	const std::vector<double> &  numeraires_BB       = atpSwaptionStructuredData.get_NUMERAIRE()        ;

	const ATMSwaptionStructuredData::RealVector &     swaprates_BB         = atpSwaptionStructuredData.get_SWAPRATE_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     swaptionVolVector_BB = atpSwaptionStructuredData.get_VOLATILITY_VECTOR() ;
	const ATMSwaptionStructuredData::SwaptionVector & swaptionVector_BB    = atpSwaptionStructuredData.get_SWAPTION_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     annuityVector_BB     = atpSwaptionStructuredData.get_ANNUITY_0_VECTOR()  ;
	const ATMSwaptionStructuredData::RealMatrix&      omega0Matrix_BB      = atpSwaptionStructuredData.get_OMEGA_0_MATRIX()    ;

	BOOST_CHECK( atpSwaptionStructuredData.check_data_consistance() );

	atpSwaptionStructuredData.print("unitTest_ATMSwaptionStructuredData1.csv");
}


BOOST_AUTO_TEST_CASE(test_ATMSwaptionStructured2)
{	
	Tenor tenorlmm = Tenor::_6M;
	Tenor tenorfixedleg = Tenor::_6M;
	Tenor tenorfloatleg = Tenor::_6M;
	size_t nbYear = 10;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorlmm, nbYear) );

	ATMSwaptionStructuredData atpSwaptionStructuredData( pLmmTenorStructure );

	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_2.csv";

	ATMSwaptionMarketData atmSwaptionMarketData;
	atmSwaptionMarketData.readDataFromFile( path_FileIn );

	atpSwaptionStructuredData.parseFromMarketData(atmSwaptionMarketData , tenorfixedleg, tenorfloatleg);

	const std::vector<double> &  lmmTimeLine         = pLmmTenorStructure->get_tenorDate()              ;
	const std::vector<double> &  libor_startdates_BB = atpSwaptionStructuredData.get_LIBOR_STARTDATES() ;
	const std::vector<double> &  libors_BB           = atpSwaptionStructuredData.get_LIBOR_INIT()       ;
	const std::vector<double> &  libors_shift_BB     = atpSwaptionStructuredData.get_LIBOR_SHIFT()      ;	
	const std::vector<double> &  zc_Maturities_BB    = atpSwaptionStructuredData.get_ZC_MATURITIES()    ;
	const std::vector<double> &  zc_BB               = atpSwaptionStructuredData.get_ZC_BOND()          ;
	const std::vector<double> &  numeraires_BB       = atpSwaptionStructuredData.get_NUMERAIRE()        ;

	const ATMSwaptionStructuredData::RealVector &     swaprates_BB         = atpSwaptionStructuredData.get_SWAPRATE_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     swaptionVolVector_BB = atpSwaptionStructuredData.get_VOLATILITY_VECTOR() ;
	const ATMSwaptionStructuredData::SwaptionVector & swaptionVector_BB    = atpSwaptionStructuredData.get_SWAPTION_VECTOR()   ;
	const ATMSwaptionStructuredData::RealVector &     annuityVector_BB     = atpSwaptionStructuredData.get_ANNUITY_0_VECTOR()  ;
	const ATMSwaptionStructuredData::RealMatrix&      omega0Matrix_BB      = atpSwaptionStructuredData.get_OMEGA_0_MATRIX()    ;

	BOOST_CHECK( atpSwaptionStructuredData.check_data_consistance() );

	atpSwaptionStructuredData.print("unitTest_ATMSwaptionStructuredData2.csv");
}

BOOST_AUTO_TEST_SUITE_END()
