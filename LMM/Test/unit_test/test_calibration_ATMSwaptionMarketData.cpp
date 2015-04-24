#define BOOST_TEST_MODULE LMM_unit_test_calibration_ATMSwaptionMarketData
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/ATMSwaptionStructuredData.h>



BOOST_AUTO_TEST_SUITE(lmm_test_calibration_ATMSwaptionMarketData)

	BOOST_AUTO_TEST_CASE(test_ATMSwaptionMarketData1)
{	
	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_1.csv";
	std::cout<< "Input ATM Swaption DataFile  :"<< path_FileIn << std::endl;

	ATMSwaptionMarketData atmSwaptionData;

	atmSwaptionData.readDataFromFile( path_FileIn );

	const std::vector<double> &  libors_loaded    = atmSwaptionData.get_LIBOR();
	const std::vector<double> &  zc_loaded        = atmSwaptionData.get_ZC_BOND();
	const std::vector<double> &  zc_Maturities    = atmSwaptionData.get_ZC_MATURITIES();
	const std::vector<double> &  libor_Maturities = atmSwaptionData.get_LIBOR_ENDDATES();

	const std::vector<double> &  swpnMaturities_loaded                 = atmSwaptionData.get_SWPM_MATURITIES();
	const std::vector<double> &  swpnTerms_loaded                      = atmSwaptionData.get_SWPM_TERMS();
	const ATMSwaptionMarketData::RealMatrix & swaprates_loaded         = atmSwaptionData.get_SWAPRATE_MATRIX();
	const ATMSwaptionMarketData::RealMatrix & swaptionVolMatrix_loaded = atmSwaptionData.get_VOLATILITY_MATRIX();

	BOOST_CHECK(libors_loaded.size() == 4);
	BOOST_CHECK(libors_loaded[0] == 0.020);
	BOOST_CHECK(libors_loaded[1] == 0.023);
	BOOST_CHECK(libors_loaded[2] == 0.024);
	BOOST_CHECK(libors_loaded[3] == 0.026);

	BOOST_CHECK(zc_loaded.size() == 4);
	BOOST_CHECK(zc_loaded[0] == 0.99);
	BOOST_CHECK(zc_loaded[1] == 0.97);
	BOOST_CHECK(zc_loaded[2] == 0.95);
	BOOST_CHECK(zc_loaded[3] == 0.93);

	BOOST_CHECK(zc_Maturities.size() == 4);
	BOOST_CHECK(zc_Maturities[0] == 0.1);
	BOOST_CHECK(zc_Maturities[1] == 0.2);
	BOOST_CHECK(zc_Maturities[2] == 1.0);
	BOOST_CHECK(zc_Maturities[3] == 2.0);

	BOOST_CHECK(libor_Maturities.size() == 4);
	BOOST_CHECK(libor_Maturities[0] == 0.1);
	BOOST_CHECK(libor_Maturities[1] == 0.2);
	BOOST_CHECK(libor_Maturities[2] == 1.0);
	BOOST_CHECK(libor_Maturities[3] == 2.0);

	BOOST_CHECK(swpnMaturities_loaded.size() == 19 ); // test the Swaption Maturities the two extrem elements
	BOOST_CHECK(swpnMaturities_loaded[0]  == 1./12. ) ;
	BOOST_CHECK(swpnMaturities_loaded[4]  == 1.     ) ;
	BOOST_CHECK(swpnMaturities_loaded[18] == 30     ) ;

	BOOST_CHECK(swpnTerms_loaded.size() == 15 ); // test the Swaption Terms the two extrem elements
	BOOST_CHECK(swpnTerms_loaded[0]  == 1  ) ; 
	BOOST_CHECK(swpnTerms_loaded[11] == 15 ) ;
	BOOST_CHECK(swpnTerms_loaded[14] == 30 ) ;

	//check the 4 cases at corners of each matrices is enough
	BOOST_CHECK(swaprates_loaded.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaprates_loaded[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaprates_loaded[0][0]  == 0.4196 ) ; BOOST_CHECK( swaprates_loaded[0][14]  == 2.7584 );
	BOOST_CHECK(swaprates_loaded[18][0] == 2.8271 ) ; BOOST_CHECK( swaprates_loaded[18][14] == 2.6174 );


	BOOST_CHECK(swaptionVolMatrix_loaded.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaptionVolMatrix_loaded[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaptionVolMatrix_loaded[0][0]  == 0.2535 ) ; BOOST_CHECK_CLOSE( swaptionVolMatrix_loaded[0][14] , 0.5860, 1e-10 );;// verry small error when comparing double
	BOOST_CHECK(swaptionVolMatrix_loaded[18][0] == 0.5663 ) ; BOOST_CHECK( swaptionVolMatrix_loaded[18][14] == 0.3135 );

	atmSwaptionData.print("unitTest_ATMSwaptionMarketData1.csv");
}


BOOST_AUTO_TEST_CASE(test_ATMSwaptionMarketData2)
{

	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_2.csv";
	std::cout<< "Input ATM Swaption DataFile  :"<< path_FileIn << std::endl;

	ATMSwaptionMarketData atmSwaptionData;

	atmSwaptionData.readDataFromFile( path_FileIn );

	const std::vector<double> &  libors_loaded    = atmSwaptionData.get_LIBOR();
	const std::vector<double> &  zc_loaded        = atmSwaptionData.get_ZC_BOND();
	const std::vector<double> &  zc_Maturities    = atmSwaptionData.get_ZC_MATURITIES();
	const std::vector<double> &  libor_Maturities = atmSwaptionData.get_LIBOR_ENDDATES();

	const std::vector<double> &  swpnMaturities_loaded                 = atmSwaptionData.get_SWPM_MATURITIES();
	const std::vector<double> &  swpnTerms_loaded                      = atmSwaptionData.get_SWPM_TERMS();
	const ATMSwaptionMarketData::RealMatrix & swaprates_loaded         = atmSwaptionData.get_SWAPRATE_MATRIX();
	const ATMSwaptionMarketData::RealMatrix & swaptionVolMatrix_loaded = atmSwaptionData.get_VOLATILITY_MATRIX();

	BOOST_CHECK(libors_loaded.size() == 22  );
	BOOST_CHECK(libors_loaded[0]  == 0.2494 );
	BOOST_CHECK(libors_loaded[21] == 3.4677 );

	BOOST_CHECK(zc_loaded.size() == 35);
	BOOST_CHECK(zc_loaded[0]  == 0.999989 );
	BOOST_CHECK(zc_loaded[34] == 0.802193 );

	BOOST_CHECK(zc_Maturities.size() == 35  );
	BOOST_CHECK(zc_Maturities[0]  == 1./360.);
	BOOST_CHECK(zc_Maturities[34] == 11.    );

	BOOST_CHECK(libor_Maturities.size() == 22  );
	BOOST_CHECK(libor_Maturities[0]     == 0.5 );
	BOOST_CHECK(libor_Maturities[21]    == 11. );

	BOOST_CHECK(swpnMaturities_loaded.size() == 19 ); // test the Swaption Maturities the two extrem elements
	BOOST_CHECK(swpnMaturities_loaded[0]  == 1./12.) ;
	BOOST_CHECK(swpnMaturities_loaded[18] == 30.   ) ;

	BOOST_CHECK(swpnTerms_loaded.size() == 15 ); // test the Swaption Terms the two extrem elements
	BOOST_CHECK(swpnTerms_loaded[0]  ==  1  ) ; 
	BOOST_CHECK(swpnTerms_loaded[14] == 30. ) ;

	//check the 4 cases at corners of each matrices is enough
	BOOST_CHECK(swaprates_loaded.size()    == 19  ) ; // nb Lines  == 19
	BOOST_CHECK(swaprates_loaded[0].size() == 15  ) ; // nb Column == 15
	BOOST_CHECK(swaprates_loaded[0][0]  == 0.4196 ) ; BOOST_CHECK( swaprates_loaded[0][14]  == 2.7584 );
	BOOST_CHECK(swaprates_loaded[18][0] == 2.8271 ) ; BOOST_CHECK( swaprates_loaded[18][14] == 2.6174 );


	BOOST_CHECK(swaptionVolMatrix_loaded.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaptionVolMatrix_loaded[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaptionVolMatrix_loaded[0][0]  == 0.2535 ) ; BOOST_CHECK_CLOSE( swaptionVolMatrix_loaded[0][14] , 0.5860 , 1e-10 );// verry small error when comparing double
	BOOST_CHECK(swaptionVolMatrix_loaded[18][0] == 0.5663 ) ; BOOST_CHECK( swaptionVolMatrix_loaded[18][14] == 0.3135 );


	atmSwaptionData.print("unitTest_ATMSwaptionMarketData2.csv");
}

BOOST_AUTO_TEST_CASE(test_ATMSwaptionMarketData_SmallData)
{	
	std::string path_FileIn = LMM::get_runtime_datapath() + "testATMSwaption_Mtk_info.csv";
	std::cout<< "Input ATM Swaption DataFile  :"<< path_FileIn << std::endl;

	ATMSwaptionMarketData atmSwaptionData;

	atmSwaptionData.readDataFromFile( path_FileIn );

	const std::vector<double> &  libors_loaded    = atmSwaptionData.get_LIBOR();
	const std::vector<double> &  zc_loaded        = atmSwaptionData.get_ZC_BOND();
	const std::vector<double> &  zc_Maturities    = atmSwaptionData.get_ZC_MATURITIES();
	const std::vector<double> &  libor_Maturities = atmSwaptionData.get_LIBOR_ENDDATES();

	const std::vector<double> &  swpnMaturities_loaded                 = atmSwaptionData.get_SWPM_MATURITIES();
	const std::vector<double> &  swpnTerms_loaded                      = atmSwaptionData.get_SWPM_TERMS();
	const ATMSwaptionMarketData::RealMatrix & swaprates_loaded         = atmSwaptionData.get_SWAPRATE_MATRIX();
	const ATMSwaptionMarketData::RealMatrix & swaptionVolMatrix_loaded = atmSwaptionData.get_VOLATILITY_MATRIX();

	BOOST_CHECK(libors_loaded.size() == 8);
	BOOST_CHECK(libors_loaded[0] == 0.2494);
	BOOST_CHECK(libors_loaded[7] == 1.5888);

	BOOST_CHECK(zc_loaded.size() == 15);
	BOOST_CHECK(zc_loaded[0]  == 0.999081);
	BOOST_CHECK(zc_loaded[14] == 0.802193);


	BOOST_CHECK(zc_Maturities.size() == 15);
	BOOST_CHECK(zc_Maturities[0]  == 0.5);
	BOOST_CHECK(zc_Maturities[14] == 11);


	BOOST_CHECK(libor_Maturities.size() == 8);
	BOOST_CHECK(libor_Maturities[0] == 0.5);
	BOOST_CHECK(libor_Maturities[7] == 4);

	BOOST_CHECK(swpnMaturities_loaded.size() == 6 ); // test the Swaption Maturities the two extrem elements
	BOOST_CHECK(swpnMaturities_loaded[0]  == 0.5 ) ;
	BOOST_CHECK(swpnMaturities_loaded[5]  == 5   ) ;

	BOOST_CHECK(swpnTerms_loaded.size() == 6 ); // test the Swaption Terms the two extrem elements
	BOOST_CHECK(swpnTerms_loaded[0] == 1 ) ; 
	BOOST_CHECK(swpnTerms_loaded[5] == 6 ) ;

	//check the 4 cases at corners of each matrices is enough
	BOOST_CHECK(swaprates_loaded.size()    ==  6 ) ; // nb Lines  == 6
	BOOST_CHECK(swaprates_loaded[0].size() ==  6 ) ; // nb Column == 6
	BOOST_CHECK(swaprates_loaded[0][0]  == 0.5372 ) ; BOOST_CHECK( swaprates_loaded[0][5] == 1.6753 );
	BOOST_CHECK(swaprates_loaded[5][0]  == 2.6363 ) ; BOOST_CHECK( swaprates_loaded[5][5] == 3.1976 );


	BOOST_CHECK(swaptionVolMatrix_loaded.size()    == 6 ) ; // nb Lines  == 6
	BOOST_CHECK(swaptionVolMatrix_loaded[0].size() == 6 ) ; // nb Column == 6
	BOOST_CHECK(swaptionVolMatrix_loaded[0][0] == 0.3146 ) ; BOOST_CHECK( swaptionVolMatrix_loaded[0][5] == 0.6758 );
	BOOST_CHECK(swaptionVolMatrix_loaded[5][0] == 0.9053 ) ; BOOST_CHECK_CLOSE( swaptionVolMatrix_loaded[5][5] , 0.8716, 1e-10 );// very small error when comparing double

	atmSwaptionData.print("unitTest_ATMSwaptionMarketData_smalldata.csv");
}


BOOST_AUTO_TEST_CASE(test_ATMSwaptionMarketData_clear)
{

	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_1.csv";

	ATMSwaptionMarketData atmSwaptionData;

	atmSwaptionData.readDataFromFile( path_FileIn );

	{   //first read and clear, check if everything is clean

		const std::vector<double> &  libors_loaded    = atmSwaptionData.get_LIBOR();
		const std::vector<double> &  zc_loaded        = atmSwaptionData.get_ZC_BOND();
		const std::vector<double> &  zc_Maturities    = atmSwaptionData.get_ZC_MATURITIES();
		const std::vector<double> &  libor_Maturities = atmSwaptionData.get_ZC_MATURITIES();

		const std::vector<double> &  swpnMaturities_loaded                 = atmSwaptionData.get_SWPM_MATURITIES();
		const std::vector<double> &  swpnTerms_loaded                      = atmSwaptionData.get_SWPM_TERMS();
		const ATMSwaptionMarketData::RealMatrix & swaprates_loaded         = atmSwaptionData.get_SWAPRATE_MATRIX();
		const ATMSwaptionMarketData::RealMatrix & swaptionVolMatrix_loaded = atmSwaptionData.get_VOLATILITY_MATRIX();

		BOOST_CHECK(libors_loaded.size()            != 0  );
		BOOST_CHECK(zc_loaded.size()                != 0  );
		BOOST_CHECK(zc_Maturities.size()            != 0  );
		BOOST_CHECK(libor_Maturities.size()         != 0  );
		BOOST_CHECK(swpnMaturities_loaded.size()    != 0  );
		BOOST_CHECK(swpnTerms_loaded.size()         != 0  );
		BOOST_CHECK(swaprates_loaded.size()         != 0  );
		BOOST_CHECK(swaptionVolMatrix_loaded.size() != 0  );


		atmSwaptionData.clear_all();

		BOOST_CHECK(libors_loaded.size()            == 0  );
		BOOST_CHECK(zc_loaded.size()                == 0  );
		BOOST_CHECK(zc_Maturities.size()            == 0  );
		BOOST_CHECK(libor_Maturities.size()         == 0  );
		BOOST_CHECK(swpnMaturities_loaded.size()    == 0  );
		BOOST_CHECK(swpnTerms_loaded.size()         == 0  );
		BOOST_CHECK(swaprates_loaded.size()         == 0  );
		BOOST_CHECK(swaptionVolMatrix_loaded.size() == 0  );
	}

	{   // after clear, reload data, check if everything read

		atmSwaptionData.readDataFromFile( path_FileIn );

		const std::vector<double> &  libors_loaded    = atmSwaptionData.get_LIBOR();
		const std::vector<double> &  zc_loaded        = atmSwaptionData.get_ZC_BOND();
		const std::vector<double> &  zc_Maturities    = atmSwaptionData.get_ZC_MATURITIES();
		const std::vector<double> &  libor_Maturities = atmSwaptionData.get_ZC_MATURITIES();

		const std::vector<double> &  swpnMaturities_loaded                 = atmSwaptionData.get_SWPM_MATURITIES();
		const std::vector<double> &  swpnTerms_loaded                      = atmSwaptionData.get_SWPM_TERMS();
		const ATMSwaptionMarketData::RealMatrix & swaprates_loaded         = atmSwaptionData.get_SWAPRATE_MATRIX();
		const ATMSwaptionMarketData::RealMatrix & swaptionVolMatrix_loaded = atmSwaptionData.get_VOLATILITY_MATRIX();


		BOOST_CHECK(libors_loaded.size()            != 0  );
		BOOST_CHECK(zc_loaded.size()                != 0  );
		BOOST_CHECK(zc_Maturities.size()            != 0  );
		BOOST_CHECK(libor_Maturities.size()         != 0  );
		BOOST_CHECK(swpnMaturities_loaded.size()    != 0  );
		BOOST_CHECK(swpnTerms_loaded.size()         != 0  );
		BOOST_CHECK(swaprates_loaded.size()         != 0  );
		BOOST_CHECK(swaptionVolMatrix_loaded.size() != 0  );
	}
}
BOOST_AUTO_TEST_SUITE_END()