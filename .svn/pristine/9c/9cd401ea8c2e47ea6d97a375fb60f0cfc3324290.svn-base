#define BOOST_TEST_MODULE LMM_unit_test_helper_MarketInfoCollector
#include <boost/test/included/unit_test.hpp>

#include <LMM/generic_path.h>

#include <LMM/helper/MarketInfoCollector.h>

BOOST_AUTO_TEST_SUITE(lmm_test_helper_marketinfocollector)
	
BOOST_AUTO_TEST_CASE(test_marketinfocollector1)
{	
	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_1.csv";
	std::cout<< "Input ATM Swaption DataFile  :"<< path_FileIn << std::endl;
	
	MarketInfoCollector::readMarketInfo( path_FileIn );
	
	std::vector<double> libors_BB     = MarketInfoCollector::get_libors();
	std::vector<double> zc_BB         = MarketInfoCollector::get_zcBonds();
	std::vector<double> zc_Maturities = MarketInfoCollector::get_zcMaturities();
	
	std::vector<double> swpnMaturities_BB          = MarketInfoCollector::get_swpnMaturities();
	std::vector<double> swpnTerms_BB               = MarketInfoCollector::get_swpnTerms();
	MarketInfoCollector::matrix_ swaprates_BB      = MarketInfoCollector::get_swapRates();
	MarketInfoCollector::matrix_ swaptionMatrix_BB = MarketInfoCollector::get_swaptionVolatilityMatrix();
	
	BOOST_CHECK(libors_BB.size() == 4);
	BOOST_CHECK(libors_BB[0] == 0.020);
	BOOST_CHECK(libors_BB[1] == 0.023);
	BOOST_CHECK(libors_BB[2] == 0.024);
	BOOST_CHECK(libors_BB[3] == 0.026);

	BOOST_CHECK(zc_BB.size() == 4);
	BOOST_CHECK(zc_BB[0] == 0.99);
	BOOST_CHECK(zc_BB[1] == 0.97);
	BOOST_CHECK(zc_BB[2] == 0.95);
	BOOST_CHECK(zc_BB[3] == 0.93);

	BOOST_CHECK(zc_Maturities.size() == 4);
	BOOST_CHECK(zc_Maturities[0] == 0.1);
	BOOST_CHECK(zc_Maturities[1] == 0.2);
	BOOST_CHECK(zc_Maturities[2] == 1.0);
	BOOST_CHECK(zc_Maturities[3] == 2.0);


	BOOST_CHECK(swpnMaturities_BB.size() == 19 ); // test the Swaption Maturities the two extrem elements
	BOOST_CHECK(swpnMaturities_BB[0]  == 1./12. ) ;
	BOOST_CHECK(swpnMaturities_BB[4]  == 1.     ) ;
	BOOST_CHECK(swpnMaturities_BB[18] == 30     ) ;
	
	BOOST_CHECK(swpnTerms_BB.size() == 15 ); // test the Swaption Terms the two extrem elements
	BOOST_CHECK(swpnTerms_BB[0]  == 1  ) ; 
	BOOST_CHECK(swpnTerms_BB[11] == 15 ) ;
	BOOST_CHECK(swpnTerms_BB[14] == 30 ) ;

	//check the 4 cases at corners of each matrices is enough
	BOOST_CHECK(swaprates_BB.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaprates_BB[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaprates_BB[0][0]  == 0.4196 ) ; BOOST_CHECK( swaprates_BB[0][14]  == 2.7584 );
	BOOST_CHECK(swaprates_BB[18][0] == 2.8271 ) ; BOOST_CHECK( swaprates_BB[18][14] == 2.6174 );


	BOOST_CHECK(swaptionMatrix_BB.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaptionMatrix_BB[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaptionMatrix_BB[0][0]  == 25.35 ) ; BOOST_CHECK( swaptionMatrix_BB[0][14]  == 58.60 );
	BOOST_CHECK(swaptionMatrix_BB[18][0] == 56.63 ) ; BOOST_CHECK( swaptionMatrix_BB[18][14] == 31.35 );
	
	MarketInfoCollector::print("unitTest_MarketInfoCollector1.csv");
}


BOOST_AUTO_TEST_CASE(test_marketinfocollector2)
{
	
	std::string path_FileIn = LMM::get_runtime_datapath() + "Mkt_info_2.csv";
	std::cout<< "Input ATM Swaption DataFile  :"<< path_FileIn << std::endl;
	
	MarketInfoCollector::readMarketInfo( path_FileIn );


	std::vector<double> libors_BB     = MarketInfoCollector::get_libors();
	std::vector<double> zc_BB         = MarketInfoCollector::get_zcBonds();
	std::vector<double> zc_Maturities = MarketInfoCollector::get_zcMaturities();
	
	std::vector<double> swpnMaturities_BB          = MarketInfoCollector::get_swpnMaturities();
	std::vector<double> swpnTerms_BB               = MarketInfoCollector::get_swpnTerms();
	MarketInfoCollector::matrix_ swaprates_BB      = MarketInfoCollector::get_swapRates();
	MarketInfoCollector::matrix_ swaptionMatrix_BB = MarketInfoCollector::get_swaptionVolatilityMatrix();
	
	BOOST_CHECK(libors_BB.size() == 22);
	BOOST_CHECK(libors_BB[0] == 0.2494);
	BOOST_CHECK(libors_BB[21] == 3.4677);

	BOOST_CHECK(zc_BB.size() == 35);
	BOOST_CHECK(zc_BB[0] == 0.999989);
	BOOST_CHECK(zc_BB[34] == 0.802193);

	BOOST_CHECK(zc_Maturities.size() == 35);
	BOOST_CHECK(zc_Maturities[0] == 1./360.);
	BOOST_CHECK(zc_Maturities[34] == 11.);

	BOOST_CHECK(swpnMaturities_BB.size() == 19 ); // test the Swaption Maturities the two extrem elements
	BOOST_CHECK(swpnMaturities_BB[0]  == 1./12. ) ;
	BOOST_CHECK(swpnMaturities_BB[18] == 30.     ) ;
	
	BOOST_CHECK(swpnTerms_BB.size() == 15 ); // test the Swaption Terms the two extrem elements
	BOOST_CHECK(swpnTerms_BB[0]  == 1  ) ; 
	BOOST_CHECK(swpnTerms_BB[14] == 30. ) ;

	//check the 4 cases at corners of each matrices is enough
	BOOST_CHECK(swaprates_BB.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaprates_BB[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaprates_BB[0][0]  == 0.4196 ) ; BOOST_CHECK( swaprates_BB[0][14]  == 2.7584 );
	BOOST_CHECK(swaprates_BB[18][0] == 2.8271 ) ; BOOST_CHECK( swaprates_BB[18][14] == 2.6174 );


	BOOST_CHECK(swaptionMatrix_BB.size()    == 19 ) ; // nb Lines  == 19
	BOOST_CHECK(swaptionMatrix_BB[0].size() == 15 ) ; // nb Column == 15
	BOOST_CHECK(swaptionMatrix_BB[0][0]  == 25.35 ) ; BOOST_CHECK( swaptionMatrix_BB[0][14]  == 58.60 );
	BOOST_CHECK(swaptionMatrix_BB[18][0] == 56.63 ) ; BOOST_CHECK( swaptionMatrix_BB[18][14] == 31.35 );
	

	MarketInfoCollector::print("unitTest_MarketInfoCollector2.csv");
}

BOOST_AUTO_TEST_SUITE_END()
